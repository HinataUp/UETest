# C 章节 - GameplayAbility 完整覆盖 (C15-C21)

## 模块总览
GA 是行为驱动战斗的核心。本章节穷举生命周期、Instancing、网络、Tag、激活方式。

| Task | GA 类 | 主题 |
| --- | --- | --- |
| C15 | `UGA_TestFire` | 最小生命周期：Activate→Commit→End |
| C16 | `UGA_PerActor / UGA_PerExecution` | 三种 InstancingPolicy |
| C17 | 四个 `UGA_Net*` | 四种 NetExecutionPolicy |
| C18 | `UGA_CheckChain` | CanActivate→Activate→Commit 检查链 |
| C19 | `UGA_Fireball` + Cost/CD GE | Cost & Cooldown 标准模式 |
| C20 | `UGA_Dash / UGA_FireballSkill` | GA Tag 五件套 |
| C21 | `UGA_TriggerByEvent / UGA_OneShot` | 五种激活方式 |

## 使用方法

**给玩家授予 GA：**
```cpp
// Server 端
FGameplayAbilitySpec Spec(UGA_Fireball::StaticClass(), 1);
ASC->GiveAbility(Spec);
```

**激活 GA（5 种方式）：**
```cpp
// 1. ByClass
ASC->TryActivateAbilityByClass(UGA_Fireball::StaticClass());

// 2. ByTag
ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_Ability_Skill_Fireball));

// 3. ByHandle
ASC->TryActivateAbility(SpecHandle);

// 4. ByEvent
FGameplayEventData EventData;
UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
    Avatar, TAG_Event_TriggerFireball, EventData);

// 5. OneShot
ASC->GiveAbilityAndActivateOnce(Spec);
```

**自动化测试：** `UETest.GAS.C_GameplayAbility与D_伤害`

## 实现要点

**InstancingPolicy 选型：**
| 策略 | 行为 | 适用 |
| --- | --- | --- |
| InstancedPerActor | 每 Actor 一实例，状态复用 | **99% 场景** |
| InstancedPerExecution | 每次激活 new ，完全隔离 | 极少数需要并发的场景 |
| NonInstanced | CDO 共享，反模式 | 仅装饰性，**不能绑 AbilityTask** |

**NetExecutionPolicy 选型：**
| 策略 | 玩家主动 | AI 自动 | 防作弊 |
| --- | --- | --- | --- |
| LocalPredicted | ✓ 首选 | ✗ | 中 |
| ServerOnly | ✗ | ✓ | 高 |
| ServerInitiated | ✗ | ✓ | 高 |
| LocalOnly | 单机 | ✗ | 无 |

**Cost / Cooldown 标准模式：**
```cpp
// 在 GA 构造函数里绑定
CostGameplayEffectClass = UGE_FireballCost::StaticClass();      // Instant 扣资源
CooldownGameplayEffectClass = UGE_FireballCD::StaticClass();    // Duration 授予 Cooldown.xxx Tag

// CommitAbility 自动应用两个 GE
// CheckCooldown 内部就是查 Cooldown.Skill.Fireball Tag 是否存在
```

**GA Tag 五件套：**
| 字段 | 作用 |
| --- | --- |
| ActivationOwnedTags | 激活期间授予自己的 Tag |
| ActivationRequiredTags | 必须有这些 Tag 才能激活 |
| ActivationBlockedTags | 有这些 Tag 时禁止激活 |
| CancelAbilitiesWithTag | 激活时取消所有带这些 Tag 的其他 GA |
| BlockAbilitiesWithTag | 激活期间禁止带这些 Tag 的其他 GA 激活 |

## 原理简述

**TryActivateAbility 内部链：**
```
TryActivateAbility(Handle)
    └── CanActivateAbility() ：早判（Tag/Cost/CD）
            ├── CheckCost()
            └── CheckCooldown()
    └── CallActivateAbility()
            └── ActivateAbility() ：你的业务代码
                    └── CommitAbility() ：再验一次 + 真正扣资源
                            ├── CommitCost()
                            └── CommitCooldown()
```

**蓄力技能模式：** `CommitCost` 与 `CommitCooldown` 可拆开调用——蓄力开始扣蓝、蓄力完成才计 CD。

**ByEvent 激活：**
- GA 构造时配 `AbilityTriggers`，TriggerSource = `GameplayEvent`
- 业务代码 `SendGameplayEventToActor(TriggerTag, EventData)`
- ASC 收到后自动 ActivateAbility ，EventData 通过 TriggerEventData 传到 GA

## 测试方法

**自动化测试覆盖：**
```
C15-C16 GA 默认配置反射检查
C17 四种 NetExecutionPolicy 反射检查
C19 Cost / Cooldown GE 类已绑定
C20 Dash 的 Required / Blocked / Owned 都已配置；Fireball Cancel Movement
```

**手动 PIE：**
- C16 ：用 PerActor GA 多次激活看 Counter 累积；用 PerExecution 看每次都是 1
- C17 ：在 Listen Server + 1 Client 下激活，看不同端的日志
- C18 ：Mana=5 时激活 Fireball（Cost=20）→ 日志显示 CheckCost=false → ActivateAbility 不被调用
- C20 ：Stunned 时按 Dash 失败；OnGround 时按 Dash 成功

## 常见坑
- NonInstanced GA 里写 `this->X = 1` → CDO 状态污染，下次激活拿到上次的值
- NonInstanced 里绑 AbilityTask → CDO 共享 Task，并发时崩
- Server RPC 配在没 Owner 的 Actor 上 → RPC 静默丢
- 忘写 EndAbility（Commit 失败也要 End！）→ GA 永久占用 ActivationOwnedTags
- 忘写 `CommitAbility` ：激活成功但不扣资源、不计 CD
- ByTag 激活会激活所有匹配的 GA（不只一个）
