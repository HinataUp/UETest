// B10 - Modifier 运算顺序：Additive / Multiplicative / Override
// 公式 ：Current = ((Base + ΣAdd) × ΠMult / ΠDiv)，Override 直接覆盖
//
// 三个 GE 类各自演示一种 ModifierOp

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "B10_GE_ModifierOrder.generated.h"

UCLASS()
class UETEST_API UGE_AtkAdd10 : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_AtkAdd10();
};

UCLASS()
class UETEST_API UGE_AtkMul1_5 : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_AtkMul1_5();
};

UCLASS()
class UETEST_API UGE_AtkOverride50 : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_AtkOverride50();
};
