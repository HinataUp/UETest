// H38 - GE 授予 / 收回 GA
// GrantedAbilities ：GE 激活期间自动 GiveAbility ，移除时按 RemovalPolicy 处理

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbility.h"
#include "H38_GE_BerserkerBuff.generated.h"

UCLASS()
class UETEST_API UGA_BerserkerSlash : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_BerserkerSlash();
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};

UCLASS()
class UETEST_API UGE_BerserkerBuff : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_BerserkerBuff();
};
