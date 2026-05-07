// A03 - GE_InitPlayerStats 实现：Instant，Override 初始数值

#include "A03_GE_InitPlayerStats.h"
#include "A01_HealthSet.h"
#include "A02_CombatSet.h"

UGE_InitPlayerStats::UGE_InitPlayerStats()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// Override：直接覆盖目标值，初始化语义最直观
	// Additive +100 起步也行，但 Override 更明确"这就是初始值"
	auto MakeOverride = [&](FGameplayAttribute Attr, float Value)
	{
		FGameplayModifierInfo Mod;
		Mod.Attribute = Attr;
		Mod.ModifierOp = EGameplayModOp::Override;
		Mod.ModifierMagnitude = FScalableFloat(Value);
		Modifiers.Add(Mod);
	};

	MakeOverride(UHealthSet::GetHealthAttribute(),       100.f);
	MakeOverride(UHealthSet::GetMaxHealthAttribute(),    100.f);
	MakeOverride(UCombatSet::GetAttackPowerAttribute(),   20.f);
	MakeOverride(UCombatSet::GetDefenseAttribute(),        0.f);
	MakeOverride(UCombatSet::GetCritRateAttribute(),       0.f);
	MakeOverride(UCombatSet::GetCritDamageAttribute(),     1.5f);
	MakeOverride(UCombatSet::GetManaAttribute(),         100.f);
	MakeOverride(UCombatSet::GetMaxManaAttribute(),      100.f);
}
