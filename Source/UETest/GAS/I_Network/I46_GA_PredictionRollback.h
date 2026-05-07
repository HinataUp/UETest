// I46 - LocalPredicted 完整闭环：预测 + 回滚
//
// 正常路径 ：Client 预测扣血 -> Server 确认 -> 一致
// 拒绝路径 ：Override CanActivateAbility 在 Server 50% 概率返回 false
//   -> Client 预测的 GE 改动被 Server 通过 PredictionKey 关联撤销
//   -> Client 看到 Health "弹回"

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "I46_GA_PredictionRollback.generated.h"

UCLASS()
class UETEST_API UGA_PredictionRollback : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_PredictionRollback();

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};
