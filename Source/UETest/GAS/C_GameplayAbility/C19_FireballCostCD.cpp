#include "C19_FireballCostCD.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "AbilitySystemComponent.h"
#include "../A_Basics/A02_CombatSet.h"
#include "../00_Common/MyGameplayTags.h"
#include "../00_Common/GASLogMacros.h"

// ───────────────── Cost：Instant 扣 Mana ─────────────────
UGE_FireballCost::UGE_FireballCost()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo Mod;
	Mod.Attribute = UCombatSet::GetManaAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;
	Mod.ModifierMagnitude = FScalableFloat(-20.f);
	Modifiers.Add(Mod);
}

// ───────────────── Cooldown：Duration 授予 Tag ─────────────────
UGE_FireballCD::UGE_FireballCD()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FScalableFloat(3.f);

	UTargetTagsGameplayEffectComponent& Comp =
		FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer Inh;
	Inh.Added.AddTag(MyGameplayTags::TAG_Cooldown_Skill_Fireball);
	Comp.SetAndApplyTargetTagChanges(Inh);
}

// ───────────────── GA：绑定 Cost / CD ─────────────────
UGA_Fireball::UGA_Fireball()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// 关键：构造期绑定 Cost / CD ，CommitAbility 会自动应用
	CostGameplayEffectClass = UGE_FireballCost::StaticClass();
	CooldownGameplayEffectClass = UGE_FireballCD::StaticClass();

	AbilityTags.AddTag(MyGameplayTags::TAG_Ability_Skill_Fireball);
}

void UGA_Fireball::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	GAS_LOG("【Fireball】Commit OK，已扣 Mana 20，CD 3s");
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
