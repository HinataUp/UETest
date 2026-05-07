// I45 - PredictionKey 观察
// 不直接操作 Key ，但要知道它存在 ：LocalPredicted GA 中能拿到非空 Key
// Server 端拿到的 Key 与 Client 端相同（通过 RPC 传递）

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "I45_GA_PredictionKey.generated.h"

UCLASS()
class UETEST_API UGA_PredictionKey : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_PredictionKey();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};
