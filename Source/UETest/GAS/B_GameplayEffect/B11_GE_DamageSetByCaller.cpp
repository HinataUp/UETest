#include "B11_GE_DamageSetByCaller.h"
#include "../A_Basics/A01_HealthSet.h"
#include "../00_Common/MyGameplayTags.h"

UGE_DamageSetByCaller::UGE_DamageSetByCaller()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo Mod;
	Mod.Attribute = UHealthSet::GetHealthAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;

	// Magnitude 类型设为 SetByCaller ，绑定指定 Tag
	FSetByCallerFloat SBC;
	SBC.DataTag = MyGameplayTags::TAG_SetByCaller_Damage;
	FGameplayEffectModifierMagnitude MagSpec(SBC);
	Mod.ModifierMagnitude = MagSpec;

	Modifiers.Add(Mod);
}
