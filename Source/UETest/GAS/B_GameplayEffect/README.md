# B 章节 - GameplayEffect 完整覆盖 (B06-B14)

## 模块总览
GE 是 GAS 数据驱动战斗的核心。本章节穷举所有常见 GE 配置，每个 task 一个 C++ GE 子类——构造函数里完成所有配置，零蓝图依赖。

| Task | GE 类 | 关键特征 |
| --- | --- | --- |
| B06 | `UGE_Heal30` | Instant，Additive +30 Health |
| B07 | `UGE_AtkBuff5s` | Duration 5s，Modifier 改 CurrentValue |
| B08 | `UGE_EquipBuff` | Infinite，三种移除方式 |
| B09 | `UGE_Poison` | Periodic 6s/2s tick |
| B10 | `UGE_AtkAdd10/Mul1_5/Override50` | 三种 ModifierOp 运算顺序 |
| B11 | `UGE_DamageSetByCaller` | SetByCaller 动态 Magnitude |
| B12 | `UGE_PoisonStack` | AggregateByTarget + 3 层叠加 |
| B13 | `UGE_Stun3s/Aura/Immunity` | Granted/Application/Ongoing 三种 Tag 条件 |
| B14 | `UGE_FireImmunity/FireDamage` | UE5.3+ 组件式免疫 |

## 使用方法

**纯代码方式（本目录采用）：**
```cpp
UGE_Heal30::UGE_Heal30()
{
    DurationPolicy = EGameplayEffectDurationType::Instant;
    FGameplayModifierInfo Mod;
    Mod.Attribute = UHealthSet::GetHealthAttribute();
    Mod.ModifierOp = EGameplayModOp::Additive;
    Mod.ModifierMagnitude = FScalableFloat(30.f);
    Modifiers.Add(Mod);
}
```

**蓝图方式（实战常用）：**
- 右键 → Blueprint Class → Parent: GameplayEffect
- Details 面板配置 Modifier / Component / Tag

**Apply 标准三步：**
```cpp
FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(GEClass, Level, Ctx);
FActiveGameplayEffectHandle Handle = ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
```

## 实现要点

**UE5.3+ Component 化配置：**
旧 GrantedTags / GrantedApplicationImmunityTags 等字段已迁移到 GameplayEffectComponent：

| 配置 | 旧字段 | 新 Component |
| --- | --- | --- |
| GrantedTags | `InheritableOwnedTagsContainer` | `UTargetTagsGameplayEffectComponent` |
| AssetTags | `InheritableGameplayEffectTags` | `UAssetTagsGameplayEffectComponent` |
| Application/Ongoing 条件 | `ApplicationTagRequirements` 字段 | `UTargetTagRequirementsGameplayEffectComponent` |
| 条件附加 GE | `TargetEffects` | `UAdditionalEffectsGameplayEffectComponent` |
| 免疫 | `GrantedApplicationImmunityTags`（已废弃）| `UImmunityGameplayEffectComponent` |
| 授予 GA | `GrantedAbilities` | `UAbilitiesGameplayEffectComponent` |

通过 `FindOrAddComponent<T>()` 取得对应 Component 后配置。

**Modifier 三种类型：**
| ModifierOp | 行为 | 用途 |
| --- | --- | --- |
| Additive | Base + 值 | 基础攻击力 +10 |
| Multiplicitive | Base × 值 | 攻击力 ×1.5 |
| Override | 直接覆盖 | 设定固定值（初始化、强制重置）|

聚合公式：`Current = ((Base + ΣAdd) × ΠMul / ΠDiv)`，**Override 直接覆盖一切**。

## 原理简述

**Duration vs Infinite vs Instant：**
| 类型 | 改 BaseValue 还是 CurrentValue | 是否进 ActiveEffects | 何时移除 |
| --- | --- | --- | --- |
| Instant | BaseValue | 否（用完即丢）| 立即 |
| Duration | CurrentValue（Modifier 聚合）| 是 | 到期自动 |
| Infinite | CurrentValue（Modifier 聚合）| 是 | 必须显式移除 |

**Periodic 本质：**
- Duration GE + Period 配置
- 每个 Period 触发一次 Instant 行为：改 BaseValue + 走 PostExecute
- 适合 DOT/HOT

**Stacking：**
- AggregateBySource ：每个施法者独立一份（3 人都施毒，目标身上 3 份）
- AggregateByTarget ：所有来源共享一份计数（3 人施毒只叠 1 份）

**Immunity（UE5.3+）：**
```cpp
UImmunityGameplayEffectComponent& Comp = FindOrAddComponent<UImmunityGameplayEffectComponent>();
FGameplayEffectQuery Query;
Query.EffectTagQuery = FGameplayTagQuery::MakeQuery_MatchAnyTags(
    FGameplayTagContainer(MyGameplayTags::TAG_Effect_Damage_Fire));
Comp.ImmunityQueries.Add(Query);
```

## 测试方法

**自动化测试：** `UETest.GAS.B_GameplayEffect`

```
B06 Instant GE：Apply 后 Health Base +30
B07 Duration GE：Current=Base+10，Base 不变
B08 Infinite GE：RemoveActiveGameplayEffect(Handle) 能正确移除
B10 Modifier 顺序：(20+10)*1.5=45；Override 50 直接覆盖
B11 SetByCaller：SetSetByCallerMagnitude(-40) 扣血正确
B14 Immunity：免疫火焰伤害时 Apply 失败
```

**手动 PIE：**
- B07 ：5 秒后看 AttackPower 自动回落
- B09 ：日志显示 6 秒内 3 次 Tick（t=0/2/4）
- B12 ：连续 Apply 5 次 ，叠层数最多 3
- B13 ：Aura GE 期间 RemoveLooseGameplayTag(InRange) 看属性挂起

## 常见坑
- 用了旧 GrantedTags 字段（已废弃）→ 编译过但运行时不生效
- Multiplicitive 拼写错误（Epic 真是这么拼的）→ 用 IDE 自动补全
- Apply Spec 时 Spec.Data 为 nullptr → 没 Make Outgoing Spec 就 Apply
- Stacking + Periodic 时 ：每 Tick 伤害 = 单层伤害 × 当前层数
- 给 Duration GE 加 Override Modifier ：行为复杂，不推荐
