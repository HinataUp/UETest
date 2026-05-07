// C15 - GA 最小生命周期：Activate → Commit → End
//
// CommitAbility = CommitCost + CommitCooldown
// 失败必须 EndAbility（否则 GA 占用槽位、ActivationOwnedTags 不释放）

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "C15_GA_TestFire.generated.h"

UCLASS()
class UETEST_API UGA_TestFire : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_TestFire();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};
