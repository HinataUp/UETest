// C18 - CanActivate → ActivateAbility → Commit 完整检查链
//
// TryActivateAbility 内部依次：
//   1. CanActivateAbility   ：Tag/Cost/CD 早判
//   2. ActivateAbility      ：业务逻辑
//   3. CommitAbility        ：再验一次 + 真正扣资源
//
// CanActivate 返回 false ：ActivateAbility 不被调用（最早拦截）
// Commit 失败 ：必须 EndAbility 还原状态

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "C18_GA_CheckChain.generated.h"

UCLASS()
class UETEST_API UGA_CheckChain : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_CheckChain();

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual bool CheckCost(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual bool CheckCooldown(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};
