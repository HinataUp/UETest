#include "D24_ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "../A_Basics/A01_HealthSet.h"
#include "../A_Basics/A02_CombatSet.h"
#include "../00_Common/GASLogMacros.h"

// 局部辅助结构 ：Caputure 定义集中管理
// 用 static struct 避免每次重复创建
struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition AttackPowerDef;
	FGameplayEffectAttributeCaptureDefinition DefenseDef;
	FGameplayEffectAttributeCaptureDefinition CritRateDef;
	FGameplayEffectAttributeCaptureDefinition CritDamageDef;

	FDamageStatics()
	{
		// Source.AttackPower (Snapshot)
		AttackPowerDef = FGameplayEffectAttributeCaptureDefinition(
			UCombatSet::GetAttackPowerAttribute(),
			EGameplayEffectAttributeCaptureSource::Source, /*bSnapshot=*/true);

		// Target.Defense (Non-Snapshot：执行时取最新)
		DefenseDef = FGameplayEffectAttributeCaptureDefinition(
			UCombatSet::GetDefenseAttribute(),
			EGameplayEffectAttributeCaptureSource::Target, /*bSnapshot=*/false);

		// Source.CritRate / CritDamage
		CritRateDef = FGameplayEffectAttributeCaptureDefinition(
			UCombatSet::GetCritRateAttribute(),
			EGameplayEffectAttributeCaptureSource::Source, /*bSnapshot=*/true);
		CritDamageDef = FGameplayEffectAttributeCaptureDefinition(
			UCombatSet::GetCritDamageAttribute(),
			EGameplayEffectAttributeCaptureSource::Source, /*bSnapshot=*/true);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics S;
	return S;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritRateDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Atk = 0.f, Def = 0.f, CritRate = 0.f, CritDmg = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, EvalParams, Atk);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseDef,     EvalParams, Def);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritRateDef,    EvalParams, CritRate);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritDamageDef,  EvalParams, CritDmg);

	// 基础伤害公式
	float BaseDamage = FMath::Max(1.f, Atk * 2.f - Def);

	// 暴击骰：在 Server 端做（ExecCalc 默认在 Server 跑）
	const bool bCrit = (FMath::FRand() < CritRate);
	if (bCrit)
	{
		BaseDamage *= CritDmg;
		GAS_LOG("【ExecCalc】暴击命中! 倍率=%.2f", CritDmg);
	}

	GAS_LOG("【ExecCalc】Atk=%.1f Def=%.1f -> Damage=%.1f", Atk, Def, BaseDamage);

	// 输出到 IncomingDamage
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		UHealthSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive,
		BaseDamage));
}
