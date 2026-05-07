// B12 - GE Stacking
// Duration 5s, Period 1s, AggregateByTarget, StackLimit=3, RefreshOnApplication
//
// 三个关键配置：
//   StackingType         ：AggregateBySource（每施法者各自一份）/ AggregateByTarget（全局共享）
//   DurationRefreshPolicy：RefreshOnSuccessfulApplication / NeverRefresh
//   ExpirationPolicy     ：ClearEntireStack / RemoveSingleStackAndRefreshDuration

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "B12_GE_PoisonStack.generated.h"

UCLASS()
class UETEST_API UGE_PoisonStack : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_PoisonStack();
};
