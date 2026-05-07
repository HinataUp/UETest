#include "B12_GE_PoisonStack.h"
#include "../A_Basics/A01_HealthSet.h"

UGE_PoisonStack::UGE_PoisonStack()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FScalableFloat(5.f);

	Period = FScalableFloat(1.f);
	bExecutePeriodicEffectOnApplication = true;

	// 堆叠配置
	StackingType = EGameplayEffectStackingType::AggregateByTarget;
	StackLimitCount = 3;
	StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::RefreshOnSuccessfulApplication;
	StackExpirationPolicy = EGameplayEffectStackingExpirationPolicy::ClearEntireStack;

	FGameplayModifierInfo Mod;
	Mod.Attribute = UHealthSet::GetHealthAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;
	// 每 Tick 伤害 = -5 ；叠 3 层后实际每 Tick = -15
	Mod.ModifierMagnitude = FScalableFloat(-5.f);
	Modifiers.Add(Mod);
}
