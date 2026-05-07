// B07 - Duration GE：限时 Modifier，到期自动移除
// 只改 CurrentValue 不改 BaseValue
// 移除后 Current 自动回落

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "B07_GE_AtkBuff5s.generated.h"

UCLASS()
class UETEST_API UGE_AtkBuff5s : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_AtkBuff5s();
};
