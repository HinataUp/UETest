#include "E28_GA_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "../00_Common/GASLogMacros.h"

UGA_WaitDelay::UGA_WaitDelay()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_WaitDelay::ActivateAbility(
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

	// 缓存上下文，供回调里使用
	CachedHandle = Handle;
	CachedActorInfo = ActorInfo;
	CachedActivationInfo = ActivationInfo;

	GAS_LOG("【WaitDelay】启动 2.0s 等待");
	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, 2.0f);
	DelayTask->OnFinish.AddDynamic(this, &UGA_WaitDelay::OnDelayFinished);
	DelayTask->ReadyForActivation();
}

void UGA_WaitDelay::OnDelayFinished()
{
	GAS_LOG("【WaitDelay】延迟完成，EndAbility");
	EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, true, false);
}
