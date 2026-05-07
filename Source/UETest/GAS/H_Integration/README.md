# H 章节 - 实战系统集成 (H38-H42)

## 模块总览
把前面章节的零件拼成"看得到、用得上"的战斗系统：授予/收回技能、条件链式效果、输入缓冲、连招、投射物。

| Task | 文件 | 主题 |
| --- | --- | --- |
| H38 | `H38_GE_BerserkerBuff.h/cpp` | GE 授予 / 收回 GA |
| H39 | `H39_ConditionalGE.h/cpp` | 条件链式效果（按 Source Tag 分支）|
| H40 | `H40_InputBuffer.h/cpp` | 输入缓冲（Tag-driven Pending Input）|
| H41 | `H41_ComboGA.h/cpp` | 连招状态机 + 取消窗口 |
| H42 | `H42_Projectile.h/cpp` | 投射物：Server 权威 + Client 视觉球 |

## 使用方法

**H38 GE 授予 GA：**
```cpp
ApplyGEToSelf(ASC, UGE_BerserkerBuff::StaticClass());
// Buff 期间 ASC 自动多了 BerserkerSlash 这个 GA
// Buff 移除（自然到期 / 主动 Remove）→ GA 也被收回（CancelAbilityImmediately）
```

**H39 条件链式 GE：**
```cpp
// Source 端打 FireType Tag
SourceASC->AddLooseGameplayTag(MyGameplayTags::TAG_Ability_FireType);

// Apply 主伤害 GE → 自动附加 BurnDOT（因为 Source 有 FireType）
ApplyGEFromSource(GE_DamageWithElement, TargetASC);
```

**H40 输入缓冲：**
```cpp
// TryActivate 失败 ：Push
if (!ASC->TryActivateAbility(...)) {
    InputBuffer->PushBufferedInput(TAG_Input_Pending_Attack, 0.3f);
}

// GA 窗口打开 ：Consume
if (InputBuffer->ConsumeBufferedInput(TAG_Input_Pending_Attack)) {
    // 进入下一段
}
```

**H42 投射物两种模式：**
- 模式A：Server SpawnActor + bReplicateMovement → Client 看到的位置滞后 RTT
- 模式B：Client 本地 SpawnActor + `MarkAsVisualOnly()` + Server RPC 真正射 → 瞬时手感 + 权威判定

## 实现要点

**H38 GrantedAbilities（UE5 Component 化）：**
```cpp
UAbilitiesGameplayEffectComponent& Comp = FindOrAddComponent<UAbilitiesGameplayEffectComponent>();
FGameplayAbilitySpecConfig Spec;
Spec.Ability = UGA_BerserkerSlash::StaticClass();
Spec.LevelScalableFloat = FScalableFloat(1.f);
Spec.RemovalPolicy = EGameplayEffectGrantedAbilityRemovePolicy::CancelAbilityImmediately;
Comp.AddGrantedAbilityConfig(Spec);
```

**RemovalPolicy 三种：**
| 策略 | 行为 |
| --- | --- |
| CancelAbilityImmediately | GE 移除时立即取消正在执行的 GA |
| RemoveAbilityOnEnd | 等 GA 自然结束后再清除 |
| DoNothing | GE 移除后 GA 仍保留 |

**H39 ConditionalEffect 配置：**
```cpp
UAdditionalEffectsGameplayEffectComponent& Comp =
    FindOrAddComponent<UAdditionalEffectsGameplayEffectComponent>();
FConditionalGameplayEffect Cond;
Cond.EffectClass = UGE_BurnDOT::StaticClass();
Cond.RequiredSourceTags.AddTag(MyGameplayTags::TAG_Ability_FireType);
Comp.OnApplicationGameplayEffects.Add(Cond);
```

**H40 输入缓冲核心思想：**
- "操作不能立刻执行时" → 用 Tag 记下来
- "条件满足时" → 检查 Tag 存在 → 消费
- 配合 Timer 实现窗口超时清除

**H41 连招 = Tag + Window Event：**
- 动画 Notify 在窗口开/关时 `SendGameplayEventToActor(WindowOpen/Close)`
- GA 内 `WaitGameplayEvent` 监听这两个事件
- 窗口期间挂 `State.Combo.Ready` Tag → 玩家按攻击键时检查此 Tag 决定是否连招

**H42 投射物视觉球关键：**
- 视觉球 ：`SetReplicates(false)` + 关碰撞 + 仅本地 Spawn
- 权威球 ：Server Spawn + 复制 + 命中判定
- 不要让视觉球做命中 ，否则 Client 与 Server 判定冲突

## 原理简述

**为什么 GE 能授予 GA：**
- ASC 内部维护 `ActivatableAbilities` 列表
- `UAbilitiesGameplayEffectComponent` 在 GE 应用时调 `GiveAbility` ，移除时调 `ClearAbility`
- 整个流程都在 Server 端 ，复制后 Client 自然同步

**条件 GE 的判定时机：**
- ExecCalc 完成后判定 RequiredSourceTags
- 满足 → 在 Target 上 Apply 附加 GE
- 不满足 → 跳过

**输入缓冲为什么用 LooseTag：**
- 输入是本地决策 ，不需要复制
- Loose Tag 计数能直接区分"按了几次"
- 配合 Timer 实现自动清除

**Server 权威 + 视觉球的本质：**
- 用本地视觉消除"按键到看到火球"的延迟
- 实际命中由 Server 一锤定音
- Client 表现层（粒子/音效）由 Cue 广播补回，所有人看到一致

## 测试方法

**自动化测试：** `UETest.GAS.G_Tag与H_集成与I_网络`
```
H38 BerserkerBuff Apply 后 ASC 含 BerserkerSlash GA
```

**手动 PIE：**
- H38 ：Apply Buff → 控制台 `showdebug abilitysystem` 看到多了一个 GA ；Buff 移除后消失
- H39 ：Source 切换 FireType / IceType ，Target 收到不同 Debuff
- H40 ：攻击动画期间狂按攻击键 → 窗口开时连续触发下一段
- H41 ：观察日志 ：Combo1 → 窗口 Open → Combo2 → 窗口 Close
- H42 ：`net.PktLag 200` 下对比两种模式手感

## 常见坑
- H38 GrantedAbilities 用旧字段（已废弃）→ 编译过但运行时不生效
- H39 ConditionalEffect 期望"目标的 Tag" 而不是 Source Tag → 看清字段名（RequiredSourceTags）
- H40 Tag 不解绑 Timer → Component 销毁后 Timer 仍持有 Component 弱引用，正常
- H41 在 Server 端发 Event ：所有 Client 都会收到 ；本地端验证用 `SendGameplayEventToActor(LocalAvatar, ...)`
- H42 视觉球做了命中判定 → Client/Server 双判定，结果不一致
