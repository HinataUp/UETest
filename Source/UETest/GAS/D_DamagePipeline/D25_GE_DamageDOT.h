// D25 - Snapshot vs Non-Snapshot 对比
// 两个版本的 DOT，只在捕获 AttackPower 时的 bSnapshot 不同

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "D25_GE_DamageDOT.generated.h"

UCLASS()
class UETEST_API UGE_DOTSnapshot : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_DOTSnapshot();
};

UCLASS()
class UETEST_API UGE_DOTNonSnapshot : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_DOTNonSnapshot();
};
