// E29 - PlayMontageAndWait + WaitGameplayEvent 组合
// 实战中最常用的 Task 配对
//
// 注意 ：本示例不实际播 Montage（需要 BP 资产），只演示骨架与回调路径

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "E29_GA_MontageEvent.generated.h"

UCLASS()
class UETEST_API UGA_MontageEvent : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_MontageEvent();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION() void OnMontageCompleted();
	UFUNCTION() void OnMontageInterrupted();
	UFUNCTION() void OnHitEventReceived(FGameplayEventData Payload);

	FGameplayAbilitySpecHandle CachedHandle;
	const FGameplayAbilityActorInfo* CachedActorInfo = nullptr;
	FGameplayAbilityActivationInfo CachedActivationInfo;
};
