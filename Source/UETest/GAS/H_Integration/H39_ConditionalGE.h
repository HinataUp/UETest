// H39 - Conditional GE：ExecCalc 成功后按 Source Tag 条件附加额外 GE
// UAdditionalEffectsGameplayEffectComponent + FConditionalGameplayEffect

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "H39_ConditionalGE.generated.h"

// 燃烧 DOT ：Source 有 Ability.FireType 时附加
UCLASS()
class UETEST_API UGE_BurnDOT : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_BurnDOT();
};

// 减速 ：Source 有 Ability.IceType 时附加
UCLASS()
class UETEST_API UGE_SlowDebuff : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_SlowDebuff();
};

// 主伤害 GE ：内含两个条件附加 GE
UCLASS()
class UETEST_API UGE_DamageWithElement : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_DamageWithElement();
};
