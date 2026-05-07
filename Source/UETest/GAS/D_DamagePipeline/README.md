# D 章节 - 伤害管线工业化 (D22-D27)

## 模块总览
工业级伤害公式骨架。从 Meta Attribute → MMC → ExecCalc → Snapshot → Crit Context → Lifesteal/Thorns，逐步扩展。

| Task | 文件 | 主题 |
| --- | --- | --- |
| D22 | `D22_GE_DamageMeta.h/cpp` + `A01_HealthSet::PostExecute` | Meta Attribute 模式 |
| D23 | `D23_MMC_DamageByAttack.h/cpp` | MMC 单 Modifier 动态数值 |
| D24 | `D24_ExecCalc_Damage.h/cpp` | ExecCalc 多属性多输出 |
| D25 | `D25_GE_DamageDOT.h/cpp` | Snapshot vs Non-Snapshot |
| D26 | `D26_MyEffectContext.h/cpp` + `MyAbilitySystemGlobals` | 自定义 EffectContext 携带暴击 |
| D27 | `D27_GE_Lifesteal.h/cpp` | 跨目标效果（吸血 + 反伤）|

## 使用方法

**典型伤害管线流程：**
```
Source GA Activate
  → MakeOutgoingSpec(GE_DamageWithExecCalc, Level)
  → Spec 设 SetByCaller / Tag
  → ApplyGameplayEffectSpecToTarget(TargetASC)
      → ExecCalc::Execute_Implementation 计算最终伤害
      → 写入 IncomingDamage Modifier
      → Target.HealthSet::PostGameplayEffectExecute 把 IncomingDamage 转为 Health 扣减
      → 同时根据 Source 的 LifeStealRate 二次 ApplyGE 给 Source 加血
```

**自动化测试：** `UETest.GAS.C_GameplayAbility与D_伤害 → D22 Meta Attribute`

## 实现要点

**Meta Attribute 模式：**
- `IncomingDamage` 不复制（无 Replicated/OnRep）
- 由 GE 写入 ；PostExecute 读取并清零
- 优势：减免公式集中在一处；可同时改 Health/Shield/Buff
- 见 `A01_HealthSet::PostGameplayEffectExecute`

**MMC vs ExecCalc：**
| 维度 | MMC | ExecCalc |
| --- | --- | --- |
| 产出 | 单个 float | 多个 ModifierEvaluatedData |
| 用途 | 给某 Modifier 提供数值 | 完整计算 Pass，写多属性 |
| 适用 | Duration/Periodic 也行 | **仅 Instant / Periodic** |
| 性能 | 轻量 | 略重，但更灵活 |

**Snapshot 取值时机：**
| bSnapshot | 取值时机 | 适用 |
| --- | --- | --- |
| true | Spec 创建时锁定 | 飞行投射物伤害（射出后不变）|
| false | 每次 Execute 时取最新 | 持续 DOT / 光环（实时反映 Source 属性变化）|

**自定义 EffectContext 三步：**
```cpp
// 1) 子类化 + override NetSerialize / Duplicate / GetScriptStruct
struct FMyGameplayEffectContext : public FGameplayEffectContext { /* ... */ };

// 2) 全局工厂
class UMyAbilitySystemGlobals : public UAbilitySystemGlobals
{
    virtual FGameplayEffectContext* AllocGameplayEffectContext() const override
    { return new FMyGameplayEffectContext(); }
};

// 3) DefaultGame.ini 配置
// [/Script/GameplayAbilities.AbilitySystemGlobals]
// AbilitySystemGlobalsClassName=/Script/UETest.MyAbilitySystemGlobals
```

## 原理简述

**为什么暴击放 Server 端 ExecCalc：**
- Client 自己骰 ：客户端可作弊
- Server 骰 ：通过 GE 复制把结果（含 EffectContext）传到 Client
- Cue 收到的 Context 中包含 `bIsCrit` ：Client 可据此播暴击特效

**跨目标效果（D27）：**
- ExecCalc 的 Output 只能改 Target 属性
- Source 的吸血需要在 PostExecute 中**二次 Apply** ：
```cpp
// 在 Target 的 PostExecute 中
if (Source.LifeStealRate > 0)
{
    auto HealSpec = Source.MakeOutgoingSpec(GE_Lifesteal);
    HealSpec.Data->SetSetByCallerMagnitude(Heal_Tag, IncomingDamage * Source.LifeStealRate);
    Source.ApplyGameplayEffectSpecToSelf(*HealSpec.Data);
}
```
- 反伤 ：标记 AssetTag `NoReflect`，避免无限循环

## 测试方法

**自动化测试：**
```
D22 通过 IncomingDamage 写入扣血 ，IncomingDamage 自动重置
```

**手动 PIE：**
- D22 ：Apply `UGE_DamageMeta` (SetByCaller -30) → Health 100→70
- D24 ExecCalc：Source.AttackPower=80，Target.Defense=30 → Damage=Max(1, 80*2-30)=130
- D25 ：Snapshot 版 DOT 在 Source.AttackPower 改变后伤害不变；Non-Snapshot 实时反映
- D26 ：CritRate=1.0 时所有伤害都暴击，Cue 端拿到 `bIsCrit=true`
- D27 ：Source.Health=80, LifeStealRate=0.2，造成 100 伤害 → Source 加 20 血变 100

## 常见坑
- ExecCalc 用在 Duration GE → 编译过但运行时无效
- 忘了调 `RelevantAttributesToCapture.Add(Def)` → 捕获定义不生效，公式拿到 0
- 自定义 Context 忘了 override `NetSerialize` → 跨网络传过去 bIsCrit=false
- 自定义 Context 忘了配 `WithNetSerializer` Trait → 序列化器不被调用
- 反伤无 NoReflect Tag → 反伤触发反伤，无限循环 stack overflow
