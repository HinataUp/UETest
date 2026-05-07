#include "E32_GA_TaskCleanup.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "../00_Common/MyGameplayTags.h"
#include "../00_Common/GASLogMacros.h"

UGA_TaskCleanup::UGA_TaskCleanup()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_TaskCleanup::ActivateAbility(
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

	// 启动两个长时间 Task ，模拟真实 GA 的多 Task 场景
	DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, 100.f);
	DelayTask->ReadyForActivation();

	EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, MyGameplayTags::TAG_Event_Montage_AttackHit);
	EventTask->ReadyForActivation();

	GAS_LOG("【TaskCleanup】两个长 Task 已启动");
}

void UGA_TaskCleanup::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	GAS_LOG("【TaskCleanup】EndAbility ：Super 会自动 EndTask 所有未完成 Task");
	// Super::EndAbility 内部会清理所有未结束的 Task
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
