// D22 - Meta Attribute 模式：通过 IncomingDamage 中转
// 实际伤害计算见 A01_HealthSet::PostGameplayEffectExecute

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "D22_GE_DamageMeta.generated.h"

UCLASS()
class UETEST_API UGE_DamageMeta : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_DamageMeta();
};
