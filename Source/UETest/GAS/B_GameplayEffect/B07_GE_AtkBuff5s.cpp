#include "B07_GE_AtkBuff5s.h"
#include "../A_Basics/A02_CombatSet.h"

UGE_AtkBuff5s::UGE_AtkBuff5s()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FScalableFloat(5.f);

	FGameplayModifierInfo Mod;
	Mod.Attribute = UCombatSet::GetAttackPowerAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;
	Mod.ModifierMagnitude = FScalableFloat(10.f);
	Modifiers.Add(Mod);
}
