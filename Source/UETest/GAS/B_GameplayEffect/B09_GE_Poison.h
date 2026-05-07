// B09 - Periodic GE (DOT)：每个 Period 执行一次 Instant 改 BaseValue
// Duration 6s, Period 2s, Health -10 / tick, ExecutePeriodicEffectOnApplication=true
// 应用瞬间立即触发一次，之后每 2s 触发一次

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "B09_GE_Poison.generated.h"

UCLASS()
class UETEST_API UGE_Poison : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_Poison();
};
