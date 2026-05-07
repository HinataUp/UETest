#include "B13_GE_StunAndAura.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "GameplayEffectComponents/TargetTagRequirementsGameplayEffectComponent.h"
#include "../A_Basics/A02_CombatSet.h"
#include "../00_Common/MyGameplayTags.h"

UGE_Stun3s::UGE_Stun3s()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FScalableFloat(3.f);

	// GrantedTags ：授予 State.Stunned
	{
		UTargetTagsGameplayEffectComponent& Comp =
			FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
		FInheritedTagContainer Inh;
		Inh.Added.AddTag(MyGameplayTags::TAG_State_Stunned);
		Comp.SetAndApplyTargetTagChanges(Inh);
	}

	// Application 条件 ：目标 不能 有 State.Immune
	{
		UTargetTagRequirementsGameplayEffectComponent& Comp =
			FindOrAddComponent<UTargetTagRequirementsGameplayEffectComponent>();
		FGameplayTagRequirements Req;
		Req.IgnoreTags.AddTag(MyGameplayTags::TAG_State_Immune);
		Comp.ApplicationTagRequirements.Add(Req);
	}
}

UGE_Aura::UGE_Aura()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	FGameplayModifierInfo Mod;
	Mod.Attribute = UCombatSet::GetAttackPowerAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;
	Mod.ModifierMagnitude = FScalableFloat(20.f);
	Modifiers.Add(Mod);

	// Ongoing 条件 ：目标必须有 State.InRange ，不满足时 GE 挂起（Inhibited）
	UTargetTagRequirementsGameplayEffectComponent& Comp =
		FindOrAddComponent<UTargetTagRequirementsGameplayEffectComponent>();
	FGameplayTagRequirements Req;
	Req.RequireTags.AddTag(MyGameplayTags::TAG_State_InRange);
	Comp.OngoingTagRequirements.Add(Req);
}

UGE_Immunity::UGE_Immunity()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	UTargetTagsGameplayEffectComponent& Comp =
		FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer Inh;
	Inh.Added.AddTag(MyGameplayTags::TAG_State_Immune);
	Comp.SetAndApplyTargetTagChanges(Inh);
}
