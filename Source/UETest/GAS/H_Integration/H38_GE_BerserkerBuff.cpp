#include "H38_GE_BerserkerBuff.h"
#include "GameplayEffectComponents/AbilitiesGameplayEffectComponent.h"
#include "AbilitySystemComponent.h"
#include "../00_Common/GASLogMacros.h"

UGA_BerserkerSlash::UGA_BerserkerSlash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_BerserkerSlash::ActivateAbility(const FGameplayAbilitySpecHandle H, const FGameplayAbilityActorInfo* AI, const FGameplayAbilityActivationInfo I, const FGameplayEventData*)
{
	if (!CommitAbility(H, AI, I)) { EndAbility(H, AI, I, true, true); return; }
	GAS_LOG("【授技能】BerserkerSlash 激活");
	EndAbility(H, AI, I, true, false);
}

UGE_BerserkerBuff::UGE_BerserkerBuff()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FScalableFloat(10.f);

	// UE5 推荐：UAbilitiesGameplayEffectComponent 替代 GrantedAbilities 数组
	UAbilitiesGameplayEffectComponent& AbilityComp =
		FindOrAddComponent<UAbilitiesGameplayEffectComponent>();
	FGameplayAbilitySpecConfig Spec;
	Spec.Ability = UGA_BerserkerSlash::StaticClass();
	Spec.LevelScalableFloat = FScalableFloat(1.f);
	Spec.RemovalPolicy = EGameplayEffectGrantedAbilityRemovePolicy::CancelAbilityImmediately;
	AbilityComp.AddGrantedAbilityConfig(Spec);
}
