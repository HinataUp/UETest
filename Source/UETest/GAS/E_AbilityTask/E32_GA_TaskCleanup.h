// E32 - AbilityTask 生命周期与并发清理
// 多 Task 并发时，EndAbility 会自动清理所有 Task
// 用成员持有 Task 指针 ，需要时可手动 EndTask 做精确清理

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "E32_GA_TaskCleanup.generated.h"

class UAbilityTask_WaitDelay;
class UAbilityTask_WaitGameplayEvent;

UCLASS()
class UETEST_API UGA_TaskCleanup : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_TaskCleanup();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> DelayTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> EventTask;
};
