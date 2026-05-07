#include "E29_GA_MontageEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "../00_Common/MyGameplayTags.h"
#include "../00_Common/GASLogMacros.h"

UGA_MontageEvent::UGA_MontageEvent()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_MontageEvent::ActivateAbility(
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

	CachedHandle = Handle;
	CachedActorInfo = ActorInfo;
	CachedActivationInfo = ActivationInfo;

	// 实战：UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(...)
	// 本示例不引入 Montage 资产；只演示 Event 监听
	UAbilityTask_WaitGameplayEvent* EventTask =
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			MyGameplayTags::TAG_Event_Montage_AttackHit,
			/*OptionalExternalTarget=*/nullptr,
			/*OnlyTriggerOnce=*/false);
	EventTask->EventReceived.AddDynamic(this, &UGA_MontageEvent::OnHitEventReceived);
	EventTask->ReadyForActivation();

	GAS_LOG("【MontageEvent】GA 启动，等待 Event.Montage.AttackHit");
}

void UGA_MontageEvent::OnMontageCompleted()
{
	GAS_LOG("【MontageEvent】OnCompleted");
	EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, true, false);
}

void UGA_MontageEvent::OnMontageInterrupted()
{
	GAS_LOG("【MontageEvent】OnInterrupted ，作为已取消处理");
	EndAbility(CachedHandle, CachedActorInfo, CachedActivationInfo, true, true);
}

void UGA_MontageEvent::OnHitEventReceived(FGameplayEventData Payload)
{
	GAS_LOG("【MontageEvent】收到命中事件 Magnitude=%.1f", Payload.EventMagnitude);
	// 实际项目此处 ApplyGameplayEffectSpecToTarget 对目标造成伤害
}
