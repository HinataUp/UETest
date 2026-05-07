#include "C15_GA_TestFire.h"
#include "AbilitySystemComponent.h"
#include "../00_Common/GASLogMacros.h"

UGA_TestFire::UGA_TestFire()
{
	// 99% 场景用 InstancedPerActor ：每个 Actor 一个实例，可保存状态
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	// LocalPredicted ：玩家主动技能首选，手感优先
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_TestFire::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// 标准开头：CommitAbility 失败必须 EndAbility
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	GAS_LOG("【GA 生命周期】Activate 在 %s 执行",
		ActorInfo && ActorInfo->IsLocallyControlled() ? TEXT("Local") : TEXT("Remote"));

	// 实际逻辑放这里 ……

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
