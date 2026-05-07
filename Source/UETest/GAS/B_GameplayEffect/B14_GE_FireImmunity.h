// B14 - GE Immunity（UImmunityGameplayEffectComponent，UE5.3+ 推荐）
//
// 旧 API：GrantedApplicationImmunityTags（已废弃）
// 新 API：UImmunityGameplayEffectComponent + ImmunityQueries

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "B14_GE_FireImmunity.generated.h"

UCLASS()
class UETEST_API UGE_FireImmunity : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_FireImmunity();
};

// 配套的火焰伤害 GE ，AssetTags 含 Effect.Damage.Fire
UCLASS()
class UETEST_API UGE_FireDamage : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_FireDamage();
};
