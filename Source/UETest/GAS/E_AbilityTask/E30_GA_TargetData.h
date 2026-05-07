// E30 - WaitTargetData：瞄准 / 命中数据
// 这里给出骨架；具体 TargetActor 类（如 GameplayAbilityTargetActor_SingleLineTrace）
// 通常在 BP 中配置或运行时 SpawnTargetActor

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "E30_GA_TargetData.generated.h"

UCLASS()
class UETEST_API UGA_TargetData : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_TargetData();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnTargetDataReady(const FGameplayAbilityTargetDataHandle& Data);

	UFUNCTION()
	void OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& Data);

	FGameplayAbilitySpecHandle CachedHandle;
	const FGameplayAbilityActorInfo* CachedActorInfo = nullptr;
	FGameplayAbilityActivationInfo CachedActivationInfo;
};
