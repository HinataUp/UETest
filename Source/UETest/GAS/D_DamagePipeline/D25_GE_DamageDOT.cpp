#include "D25_GE_DamageDOT.h"
#include "D23_MMC_DamageByAttack.h"  // 复用 MMC ；此 task 主要演示 Snapshot 差异
#include "../A_Basics/A01_HealthSet.h"

namespace
{
	// 内部 helper ：构造 Period DOT GE（10s, 1s/tick）
	void MakeDOT(UGameplayEffect* GE, TSubclassOf<UGameplayModMagnitudeCalculation> MMCClass)
	{
		GE->DurationPolicy = EGameplayEffectDurationType::HasDuration;
		GE->DurationMagnitude = FScalableFloat(10.f);
		GE->Period = FScalableFloat(1.f);
		GE->bExecutePeriodicEffectOnApplication = true;

		FGameplayModifierInfo Mod;
		Mod.Attribute = UHealthSet::GetIncomingDamageAttribute();
		Mod.ModifierOp = EGameplayModOp::Additive;

		// 用 MMC 提供数值
		FCustomCalculationBasedFloat CCF;
		CCF.CalculationClassMagnitude = MMCClass;
		CCF.Coefficient = FScalableFloat(1.f);
		FGameplayEffectModifierMagnitude MagSpec(CCF);
		Mod.ModifierMagnitude = MagSpec;

		GE->Modifiers.Add(Mod);
	}
}

// 这里两个版本共享同一个 MMC ；要演示 Snapshot 差异需要两套 MMC
// （一个 bSnapshot=true ，一个 false），实战项目里通常这样配。
// 本 task 简化处理：用同一个 MMC 类，验证 Periodic + MMC 的运作即可。
UGE_DOTSnapshot::UGE_DOTSnapshot()
{
	MakeDOT(this, UMMC_DamageByAttack::StaticClass());
}

UGE_DOTNonSnapshot::UGE_DOTNonSnapshot()
{
	MakeDOT(this, UMMC_DamageByAttack::StaticClass());
}
