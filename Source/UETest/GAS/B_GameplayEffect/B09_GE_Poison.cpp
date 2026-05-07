#include "B09_GE_Poison.h"
#include "../A_Basics/A01_HealthSet.h"

UGE_Poison::UGE_Poison()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FScalableFloat(6.f);

	Period = FScalableFloat(2.f);
	bExecutePeriodicEffectOnApplication = true;

	FGameplayModifierInfo Mod;
	Mod.Attribute = UHealthSet::GetHealthAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;
	Mod.ModifierMagnitude = FScalableFloat(-10.f);
	Modifiers.Add(Mod);
}
