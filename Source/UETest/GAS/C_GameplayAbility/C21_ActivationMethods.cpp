#include "C21_ActivationMethods.h"
#include "AbilitySystemComponent.h"
#include "../00_Common/MyGameplayTags.h"
#include "../00_Common/GASLogMacros.h"

UGA_TriggerByEvent::UGA_TriggerByEvent()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	// 配置 Trigger ：当 Avatar 收到 Event.TriggerFireball 时自动激活本 GA
	FAbilityTriggerData Trigger;
	Trigger.TriggerTag = MyGameplayTags::TAG_Event_TriggerFireball;
	Trigger.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	AbilityTriggers.Add(Trigger);
}

void UGA_TriggerByEvent::ActivateAbility(const FGameplayAbilitySpecHandle H, const FGameplayAbilityActorInfo* AI, const FGameplayAbilityActivationInfo I, const FGameplayEventData* EventData)
{
	if (!CommitAbility(H, AI, I)) { EndAbility(H, AI, I, true, true); return; }
	GAS_LOG("【激活-ByEvent】Magnitude=%.1f", EventData ? EventData->EventMagnitude : 0.f);
	EndAbility(H, AI, I, true, false);
}

UGA_OneShot::UGA_OneShot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UGA_OneShot::ActivateAbility(const FGameplayAbilitySpecHandle H, const FGameplayAbilityActorInfo* AI, const FGameplayAbilityActivationInfo I, const FGameplayEventData*)
{
	if (!CommitAbility(H, AI, I)) { EndAbility(H, AI, I, true, true); return; }
	GAS_LOG("【激活-OneShot】用一次后 GA 自动从 ASC 清除");
	EndAbility(H, AI, I, true, false);
}
