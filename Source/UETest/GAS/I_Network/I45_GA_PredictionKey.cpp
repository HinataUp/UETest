#include "I45_GA_PredictionKey.h"
#include "AbilitySystemComponent.h"
#include "../00_Common/GASLogMacros.h"

UGA_PredictionKey::UGA_PredictionKey()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_PredictionKey::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData*)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// ActivationInfo 内置 PredictionKey
	const FPredictionKey& Key = ActivationInfo.GetActivationPredictionKey();
	GAS_LOG("【PredictionKey】Current=%d IsLocalClientKey=%d IsValidKey=%d",
		Key.Current,
		Key.IsLocalClientKey() ? 1 : 0,
		Key.IsValidKey() ? 1 : 0);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
