// B13 - GE Tag 赋予 + Application/Ongoing/Removal 三种条件
//
// GE_Stun3s ：
//   * Duration 3s
//   * GrantedTags: State.Stunned
//   * ApplicationTagRequirements: 目标必须没有 State.Immune
//
// GE_Aura ：
//   * Infinite, AttackPower +20
//   * OngoingTagRequirements: 目标必须有 State.InRange
//   * 不满足时 GE 暂停（Inhibited），不移除

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "B13_GE_StunAndAura.generated.h"

UCLASS()
class UETEST_API UGE_Stun3s : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_Stun3s();
};

UCLASS()
class UETEST_API UGE_Aura : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_Aura();
};

UCLASS()
class UETEST_API UGE_Immunity : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_Immunity();
};
