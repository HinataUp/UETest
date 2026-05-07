// E28 - WaitDelay：异步等待入门
// AbilityTask 标准模式：静态工厂 → 绑定委托 → ReadyForActivation
// 整个 Wait 期间 GA 保持激活态，OwnedTags 持续生效

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "E28_GA_WaitDelay.generated.h"

UCLASS()
class UETEST_API UGA_WaitDelay : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_WaitDelay();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnDelayFinished();

	// 用成员持有 Spec 信息以便回调里 EndAbility
	FGameplayAbilitySpecHandle CachedHandle;
	const FGameplayAbilityActorInfo* CachedActorInfo = nullptr;
	FGameplayAbilityActivationInfo CachedActivationInfo;
};
