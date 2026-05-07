// C20 - GA Tag 五件套
//
// ActivationOwnedTags    ：激活期间授予自身的 Tag（GA 处于激活态时存在）
// ActivationRequiredTags ：必须有这些 Tag 才能激活
// ActivationBlockedTags  ：有这些 Tag 时禁止激活
// CancelAbilitiesWithTag ：激活时强制取消所有带这些 Tag 的其他 GA
// BlockAbilitiesWithTag  ：激活期间禁止带这些 Tag 的其他 GA 激活

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "C20_GA_TagFiveSet.generated.h"

UCLASS()
class UETEST_API UGA_Dash : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Dash();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};

// Fireball 这里独立一个版本演示 Cancel/Block ，不复用 C19
UCLASS()
class UETEST_API UGA_FireballSkill : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_FireballSkill();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};
