# C 章节 - 动画系统 (P78-P85)

## 模块总览
| Task | 主题 | 文件 |
| --- | --- | --- |
| P78 | UAnimInstance C++ 驱动 | `P78_MyAnimInstance.h/cpp` |
| P79 | State Machine + BlendSpace | 见下方"P79 步骤" |
| P80 | AnimNotify + AnimNotifyState | `P80_AnimNotify_SendEvent` + `P80_AnimNotifyState_TrailEffect` |
| P81 | IK ：FABRIK / TwoBone | 见下方"P81 笔记" |
| P82 | Linked Anim Graph 分层 | 见下方"P82 笔记" |
| P83 | Control Rig（运行时 Rig 控制）| `P83_ControlRigHelper.h/cpp` |
| P84 | Motion Matching（PoseSearch）| 见下方"P84 笔记" |
| P85 | Distance Matching + Stride Warping | 见下方"P85 笔记" |

## P78 - AnimInstance

```cpp
class UMyAnimInstance : public UAnimInstance {
    UPROPERTY(BlueprintReadOnly) float Speed = 0.f;
    UPROPERTY(BlueprintReadOnly) float Direction = 0.f;
    UPROPERTY(BlueprintReadOnly) bool bIsInAir = false;

    virtual void NativeUpdateAnimation(float DeltaSeconds) override {
        if (APawn* Pawn = TryGetPawnOwner()) {
            Speed = Pawn->GetVelocity().Size2D();
            Direction = UKismetMathLibrary::CalculateDirection(...);
            bIsInAir = Cast<ACharacter>(Pawn)->GetCharacterMovement()->IsFalling();
        }
    }
};
```

**编辑器：** New Anim Blueprint → Parent: `P78_MyAnimInstance` → AnimGraph 中读字段。

## P79 - State Machine + BlendSpace

**State Machine（AnimBP 里）：**
- 状态 ：Idle / Locomotion / JumpStart / FallLoop / Land
- Transition ：用 C++ 字段 `Speed > 10` / `bIsInAir == true` 切换

**1D BlendSpace：** Speed 轴 ：0(站立) / 200(走) / 600(跑) ；BlendSpace 自动按 Speed 插值

**2D BlendSpace：** Speed × Direction ：左前 / 右前 / 后退 全方向移动

**步骤：**
1. New → Animation → Blend Space 1D / Blend Space
2. Axis Setting 配 Sample Range
3. 拖入 Anim Sequence 到对应坐标
4. AnimBP 中拖 BlendSpace 节点 ：Speed / Direction 接 P78 字段

## P80 - AnimNotify

**两种 Notify：**
| 类型 | 触发 | 用途 |
| --- | --- | --- |
| `UAnimNotify` | 单帧瞬发 | 命中帧 / 脚步声 |
| `UAnimNotifyState` | 区间持续 | 拖尾 / 无敌帧窗口 |

**编辑器：** Anim Sequence / Montage 时间轴 → 右键 → Add Notify → 选 P80 Notify。

```cpp
// 命中帧 ：发 GameplayEvent 给 GAS
UCLASS() class UP80_AnimNotify_SendEvent : public UAnimNotify {
    UPROPERTY(EditAnywhere) FGameplayTag EventTag;
    virtual void Notify(...) override {
        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, Data);
    }
};

// 拖尾 ：Begin Spawn + End Destroy
UCLASS() class UP80_AnimNotifyState_TrailEffect : public UAnimNotifyState {
    virtual void NotifyBegin(...) override;
    virtual void NotifyTick(...) override;
    virtual void NotifyEnd(...) override;
};
```

## P81 - IK 笔记

**AnimGraph 中 IK 节点：**
| 节点 | 用途 |
| --- | --- |
| Two Bone IK | 手 / 脚 IK（链长 = 2 关节）|
| FABRIK | 任意链长（手够墙 / 脖子转向）|
| Full Body IK | 整身 IK（VR 化身）|

**典型用法：脚部地面 IK**
1. 每帧 Line Trace 从脚踝向下 ：拿到地面高度
2. 在 AnimBP 中 ：Two Bone IK 节点 → Effector Location = 调整后脚踝位置
3. C++ 中通过 `Set Foot Effector` 字段（在 AnimInstance 上）传入

## P82 - Linked Anim Graph 笔记

**用途：** 上下半身独立动画（跑步中攻击）

**步骤：**
1. 主 AnimBP ：Locomotion 层（全身移动）
2. 子 AnimBP ：Combat 层（上半身攻击叠加）
3. 主 AnimBP 中加 `Linked Anim Graph` 节点 ：引用子 AnimBP
4. 用 `Layered Blend Per Bone` 合并 ：下半身用主 BP，上半身用子 BP（按骨骼名分割）

## P83 - Control Rig

```cpp
UP83_ControlRigHelper::SetAnimFloat(MyComp, FName("SpineCurl"), 0.5f);
```

**编辑器：**
1. New → Animation → Control Rig
2. Rig Graph 定义控制点（Spine / Hand IK Effector）
3. AnimBP 中 ：Control Rig 节点 → 引用此 Rig
4. 暴露的 Variable / Curve 通过 AnimInstance 字段驱动

**实战：** 程序化脊柱弯曲 / 武器握姿 IK / 程序化 LookAt。

## P84 - Motion Matching（PoseSearch）

**Plugin：** PoseSearch（UE5.4+）

**思路：** 不用手写 State Machine + Transition ；直接对动画库做"姿态搜索"，每帧选最匹配的下一帧。

**步骤：**
1. 创建 PoseSearch Schema ：定义查询通道（速度 / 方向 / 未来轨迹）
2. 创建 PoseSearchDatabase ：导入动画片段 + 配 Schema
3. AnimBP 中 ：用 `Motion Matching` 节点替代 State Machine

**优势：** 自然过渡 / 无需手配 Transition / 大量动作时省人工

## P85 - Distance Matching + Stride Warping

**Distance Matching：** 起步 / 停步动画与实际加速 / 减速距离匹配 ；解决"动画播完了人还没停"的滑步

**Stride Warping：** 根据移动速度动态拉伸步幅 ；解决"脚踩不到地"

**AnimGraph 节点：** `Advance Time by Distance Matching` / `Stride Warping`

**实战指标：** 启用 stat anim ，看 RootMotion 与 Capsule Velocity 的差值（越小越无滑步）

## 测试方法

**自动化测试：** `UETest.Fundamentals.P3_UI_AI_动画 → C_动画`
```
P78 AnimInstance 暴露 Speed/Direction/bIsInAir
P80 Notify / NotifyState 子类正确
P83 ControlRigHelper 是 ActorComponent
```

**手动 PIE：**
- P78 ：跑动看 BlendSpace 平滑过渡 ；Anim Editor 看 Speed/Direction 实时数据
- P80 ：在 Montage 加 Notify → 播放 Montage → 命中帧触发 GameplayEvent
- P83 ：BP 调 SetAnimFloat → AnimBP 中读取传给 Control Rig

## 常见坑
- AnimInstance 子类没在 SkeletalMesh 的 AnimClass 配置 → C++ 字段不被读取
- BlendSpace Sample Position 配错 → 高速时仍播放走路
- Notify 命中帧晚 1 帧 ：动画 Tick 在物理之后 ；命中前一帧加 Notify 避免延迟
- Linked Anim Graph 子 BP 改变后主 BP 不刷新 → 重新编译主 BP
- Control Rig 性能 ：每帧重新求解 Rig Graph ；复杂 Rig 单角色就可能 1-2ms
