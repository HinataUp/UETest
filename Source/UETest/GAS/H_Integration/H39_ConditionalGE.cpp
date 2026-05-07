#include "H39_ConditionalGE.h"
#include "GameplayEffectComponents/AdditionalEffectsGameplayEffectComponent.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "../A_Basics/A01_HealthSet.h"
#include "../00_Common/MyGameplayTags.h"

UGE_BurnDOT::UGE_BurnDOT()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FScalableFloat(4.f);
	Period = FScalableFloat(1.f);
	bExecutePeriodicEffectOnApplication = false;

	FGameplayModifierInfo Mod;
	Mod.Attribute = UHealthSet::GetHealthAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;
	Mod.ModifierMagnitude = FScalableFloat(-5.f);
	Modifiers.Add(Mod);

	UTargetTagsGameplayEffectComponent& Comp =
		FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer Inh;
	Inh.Added.AddTag(MyGameplayTags::TAG_State_Burning);
	Comp.SetAndApplyTargetTagChanges(Inh);
}

UGE_SlowDebuff::UGE_SlowDebuff()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FScalableFloat(3.f);

	UTargetTagsGameplayEffectComponent& Comp =
		FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer Inh;
	Inh.Added.AddTag(MyGameplayTags::TAG_State_Slowed);
	Comp.SetAndApplyTargetTagChanges(Inh);
}

UGE_DamageWithElement::UGE_DamageWithElement()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo Mod;
	Mod.Attribute = UHealthSet::GetIncomingDamageAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;
	Mod.ModifierMagnitude = FScalableFloat(30.f);
	Modifiers.Add(Mod);

	// 配置条件附加 GE
	UAdditionalEffectsGameplayEffectComponent& AddComp =
		FindOrAddComponent<UAdditionalEffectsGameplayEffectComponent>();

	// 条件 1：Source 带 FireType -> 附加 BurnDOT
	{
		FConditionalGameplayEffect Cond;
		Cond.EffectClass = UGE_BurnDOT::StaticClass();
		Cond.RequiredSourceTags.AddTag(MyGameplayTags::TAG_Ability_FireType);
		AddComp.OnApplicationGameplayEffects.Add(Cond);
	}
	// 条件 2：Source 带 IceType -> 附加 SlowDebuff
	{
		FConditionalGameplayEffect Cond;
		Cond.EffectClass = UGE_SlowDebuff::StaticClass();
		Cond.RequiredSourceTags.AddTag(MyGameplayTags::TAG_Ability_IceType);
		AddComp.OnApplicationGameplayEffects.Add(Cond);
	}
}
