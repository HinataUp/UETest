// D26 - 全局工厂：让整个项目使用 FMyGameplayEffectContext
//
// 注册方式：DefaultGame.ini 配置
//   [/Script/GameplayAbilities.AbilitySystemGlobals]
//   AbilitySystemGlobalsClassName=/Script/UETest.MyAbilitySystemGlobals

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "D26_MyAbilitySystemGlobals.generated.h"

UCLASS()
class UETEST_API UMyAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
