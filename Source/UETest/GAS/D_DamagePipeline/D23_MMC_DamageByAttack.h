// D23 - Modifier Magnitude Calculation
//
// MMC 给单个 Modifier 提供动态数值，可读取 Source/Target 属性
// 公式：Damage = 20 + Source.AttackPower * 0.5

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "D23_MMC_DamageByAttack.generated.h"

UCLASS()
class UETEST_API UMMC_DamageByAttack : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_DamageByAttack();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	// 捕获 Source 端的 AttackPower ；Snapshot=true 表示 Spec 创建时锁值
	FGameplayEffectAttributeCaptureDefinition AttackPowerDef;
};
