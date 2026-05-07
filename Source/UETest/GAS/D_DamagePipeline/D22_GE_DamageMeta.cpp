#include "D22_GE_DamageMeta.h"
#include "../A_Basics/A01_HealthSet.h"
#include "../00_Common/MyGameplayTags.h"

UGE_DamageMeta::UGE_DamageMeta()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// 关键：写入 IncomingDamage 而不是 Health
	// PostGameplayEffectExecute 会读取 IncomingDamage、计算减免、写入 Health
	FGameplayModifierInfo Mod;
	Mod.Attribute = UHealthSet::GetIncomingDamageAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;

	// 用 SetByCaller 让外部决定具体伤害值
	FSetByCallerFloat SBC;
	SBC.DataTag = MyGameplayTags::TAG_SetByCaller_Damage;
	FGameplayEffectModifierMagnitude MagSpec(SBC);
	Mod.ModifierMagnitude = MagSpec;

	Modifiers.Add(Mod);
}
