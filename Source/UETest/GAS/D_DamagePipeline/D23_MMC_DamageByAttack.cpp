#include "D23_MMC_DamageByAttack.h"
#include "../A_Basics/A02_CombatSet.h"

UMMC_DamageByAttack::UMMC_DamageByAttack()
{
	// 配置捕获定义：从 Source 端的 AttackPower 取值，Snapshot
	AttackPowerDef.AttributeToCapture = UCombatSet::GetAttackPowerAttribute();
	AttackPowerDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttackPowerDef.bSnapshot = true;

	// 注册到 RelevantAttributesToCapture，GE 系统才会真正抓取该属性
	RelevantAttributesToCapture.Add(AttackPowerDef);
}

float UMMC_DamageByAttack::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 取出捕获到的 AttackPower
	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Atk = 0.f;
	GetCapturedAttributeMagnitude(AttackPowerDef, Spec, EvalParams, Atk);

	// 公式：Damage = 20 + Atk * 0.5
	return 20.f + Atk * 0.5f;
}
