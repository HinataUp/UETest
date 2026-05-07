#include "B10_GE_ModifierOrder.h"
#include "../A_Basics/A02_CombatSet.h"

namespace
{
	void MakeMod(UGameplayEffect* GE, EGameplayModOp::Type Op, float Value)
	{
		FGameplayModifierInfo Mod;
		Mod.Attribute = UCombatSet::GetAttackPowerAttribute();
		Mod.ModifierOp = Op;
		Mod.ModifierMagnitude = FScalableFloat(Value);
		GE->Modifiers.Add(Mod);
	}
}

UGE_AtkAdd10::UGE_AtkAdd10()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	MakeMod(this, EGameplayModOp::Additive, 10.f);
}

UGE_AtkMul1_5::UGE_AtkMul1_5()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	// 注意 ：UE5 中倍率乘 ModifierOp 命名为 Multiplicitive（Epic 拼写）；新版改成 MultiplyAdditive 等
	// 这里用 Multiplicitive 在 5.6 仍可用
	MakeMod(this, EGameplayModOp::Multiplicitive, 1.5f);
}

UGE_AtkOverride50::UGE_AtkOverride50()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	MakeMod(this, EGameplayModOp::Override, 50.f);
}
