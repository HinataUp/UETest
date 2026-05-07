#include "C16_InstancingPolicy.h"
#include "AbilitySystemComponent.h"
#include "../00_Common/GASLogMacros.h"

UGA_PerActor::UGA_PerActor()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_PerActor::ActivateAbility(
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

	++Counter;
	// PerActor ：第 1/2/3 次激活分别打 1/2/3
	GAS_LOG("【InstancingPolicy】PerActor Counter=%d", Counter);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

UGA_PerExecution::UGA_PerExecution()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
}

void UGA_PerExecution::ActivateAbility(
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

	++Counter;
	// PerExecution ：每次新实例 ，Counter 永远 1
	GAS_LOG("【InstancingPolicy】PerExecution Counter=%d (始终 1)", Counter);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
