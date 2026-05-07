#include "C18_GA_CheckChain.h"
#include "AbilitySystemComponent.h"
#include "../00_Common/GASLogMacros.h"

UGA_CheckChain::UGA_CheckChain()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

bool UGA_CheckChain::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bSuper = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	GAS_LOG("【检查链】CanActivateAbility = %s", bSuper ? TEXT("true") : TEXT("false"));
	return bSuper;
}

bool UGA_CheckChain::CheckCost(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bOK = Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
	GAS_LOG("【检查链】CheckCost = %s", bOK ? TEXT("true") : TEXT("false"));
	return bOK;
}

bool UGA_CheckChain::CheckCooldown(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bOK = Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);
	GAS_LOG("【检查链】CheckCooldown = %s", bOK ? TEXT("true") : TEXT("false"));
	return bOK;
}

void UGA_CheckChain::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	GAS_LOG("【检查链】ActivateAbility 进入");
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		GAS_LOG("【检查链】CommitAbility 失败，EndAbility");
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	GAS_LOG("【检查链】Commit 成功");
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
