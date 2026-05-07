// B06 - Instant GE：一次性改 BaseValue
// 应用后立即结束，不留在 ActiveEffects 中

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "B06_GE_Heal30.generated.h"

UCLASS()
class UETEST_API UGE_Heal30 : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_Heal30();
};
