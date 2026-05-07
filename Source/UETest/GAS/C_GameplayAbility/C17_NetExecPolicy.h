// C17 - NetExecutionPolicy 四种变体
//
// LocalOnly       ：只在本地端执行（仅装饰、单机）
// LocalPredicted  ：本地先跑、Server 随后跑（玩家主动技能首选）
// ServerOnly      ：只在 Server 跑（防作弊敏感、AI 主动）
// ServerInitiated ：Server 先发起、Client 随后跑（AI 自动触发 Buff）

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "C17_NetExecPolicy.generated.h"

UCLASS() class UETEST_API UGA_NetLocalOnly       : public UGameplayAbility { GENERATED_BODY() public: UGA_NetLocalOnly();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle, const FGameplayAbilityActorInfo*, const FGameplayAbilityActivationInfo, const FGameplayEventData*) override; };
UCLASS() class UETEST_API UGA_NetLocalPredicted  : public UGameplayAbility { GENERATED_BODY() public: UGA_NetLocalPredicted();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle, const FGameplayAbilityActorInfo*, const FGameplayAbilityActivationInfo, const FGameplayEventData*) override; };
UCLASS() class UETEST_API UGA_NetServerOnly      : public UGameplayAbility { GENERATED_BODY() public: UGA_NetServerOnly();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle, const FGameplayAbilityActorInfo*, const FGameplayAbilityActivationInfo, const FGameplayEventData*) override; };
UCLASS() class UETEST_API UGA_NetServerInitiated : public UGameplayAbility { GENERATED_BODY() public: UGA_NetServerInitiated();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle, const FGameplayAbilityActorInfo*, const FGameplayAbilityActivationInfo, const FGameplayEventData*) override; };
