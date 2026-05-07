// C19 - Cost GE + Cooldown GE + Fireball GA
//
// Cost     ：Instant，扣 Mana 20
// Cooldown ：Duration 3s，授予 Cooldown.Skill.Fireball Tag
// CheckCooldown 内部就是看这个 Tag 是否存在

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbility.h"
#include "C19_FireballCostCD.generated.h"

UCLASS()
class UETEST_API UGE_FireballCost : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_FireballCost();
};

UCLASS()
class UETEST_API UGE_FireballCD : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_FireballCD();
};

UCLASS()
class UETEST_API UGA_Fireball : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Fireball();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};
