# E 章节 - AbilityTask (E28-E32)

## 模块总览
GA 的"异步等待"原语。所有"播蒙太奇等结束"、"等命中事件"、"蓄力释放"等都通过 Task 实现。

| Task | 文件 | 主题 |
| --- | --- | --- |
| E28 | `E28_GA_WaitDelay.h/cpp` | 异步等待最简模板 |
| E29 | `E29_GA_MontageEvent.h/cpp` | PlayMontageAndWait + WaitGameplayEvent 组合 |
| E30 | `E30_GA_TargetData.h/cpp` | WaitTargetData 骨架 |
| E31 | `E31_AbilityTask_WaitChargeRelease.h/cpp` | 自定义 Task 完整模板（蓄力）|
| E32 | `E32_GA_TaskCleanup.h/cpp` | 多 Task 并发清理 |

## 使用方法

**通用 AbilityTask 三步：**
```cpp
// 1. 静态工厂
UAbilityTask_WaitDelay* Task = UAbilityTask_WaitDelay::WaitDelay(this, 2.0f);

// 2. 绑定回调
Task->OnFinish.AddDynamic(this, &UMyGA::OnDelayFinished);

// 3. ReadyForActivation
Task->ReadyForActivation();
```

期间 GA 保持激活态 ；ActivationOwnedTags 持续生效。回调里通常 `EndAbility`。

**自动化测试：** Spec 暂未覆盖（Task 涉及 Tick + 异步，单测难度高），完全靠手动 PIE。

## 实现要点

**自定义 AbilityTask 五件套：**
```cpp
class UAbilityTask_X : public UAbilityTask
{
    // 1. 静态工厂
    static UAbilityTask_X* CreateProxy(UGameplayAbility* OwningAbility, ...);

    // 2. Activate ：启动逻辑
    virtual void Activate() override;

    // 3. TickTask ：每帧（需 bTickingTask=true）
    virtual void TickTask(float DeltaTime) override;

    // 4. OnDestroy ：清理
    virtual void OnDestroy(bool bAbilityEnded) override;

    // 5. 委托广播前检查 OwningAbility 仍有效
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        OnXxx.Broadcast(...);
    }
};
```

**WaitGameplayEvent 模式：**
- AnimNotify / 业务代码 `SendGameplayEventToActor(Tag, Data)`
- GA 内 Task 监听该 Tag → `EventReceived` 回调拿到 Payload
- 解耦 ：动画系统不直接调 GA ，通过 Tag 总线

**多 Task 并发：**
- `EndAbility` 时 GAS 自动 EndTask 所有未完成 Task
- 想精确控制：用成员变量持有 Task 指针，必要时手动 `Task->EndTask()`

## 原理简述

**为什么 Task 必须用静态工厂：**
- AbilityTask 内部需要绑定到 OwningAbility（建立生命周期关联）
- 工厂 `NewAbilityTask<T>(OwningAbility)` 帮你做完关联
- 直接 NewObject 出 Task 不会被 GA 跟踪 ，EndAbility 时不会清理

**ShouldBroadcastAbilityTaskDelegates 检查的意义：**
- GA 已 Cancel 但 Task 还有未传送的回调
- 此时直接 Broadcast 会调到已无效的 GA → 崩
- 检查后才 Broadcast ：防御此场景的标准做法

**bTickingTask 的代价：**
- 启用后 Task 加入 Tick 列表，每帧调 TickTask
- 高频技能（每秒多次）要警惕 Tick 累积
- 不需要每帧逻辑时 ，用 Timer 或 Event 驱动更轻

## 测试方法

**手动 PIE：**

**E28 WaitDelay：**
- 触发 GA → 2 秒后日志 "WaitDelay 延迟完成"
- ActivationOwnedTags 在 2 秒期间持续生效

**E29 MontageEvent：**
- 触发 GA → 收到 `Event.Montage.AttackHit` 时打印 Magnitude
- 实战中由 AnimNotify 在命中帧发送 Event

**E31 WaitChargeRelease：**
- 启动 GA → 持续 Tick 累加 ChargeTime
- 满 2.0s 触发 `OnFullyCharged`
- 调用 `NotifyReleased` 触发 `OnReleased(Duration)`

**E32 TaskCleanup：**
- 启动 GA（含 100s Delay + 永久 EventTask）
- Cancel GA → 日志显示两个 Task 的 OnDestroy 都被调用

**自动化测试：** 此章节 Spec 不易覆盖，靠手动 PIE。

## 常见坑
- 直接 NewObject 出 Task 而不用静态工厂 → Task 不被 GA 跟踪，EndAbility 时不清理 → 内存泄漏
- 在回调里 EndAbility 但其他 Task 还在跑 → 注意清理顺序
- TickTask 里做重计算 → 高频技能下性能塌陷
- NonInstanced GA 上绑 Task → CDO 共享，Task 状态污染下次激活
