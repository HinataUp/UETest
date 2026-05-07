#include "D27_GE_Lifesteal.h"
#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"
#include "../A_Basics/A01_HealthSet.h"
#include "../00_Common/MyGameplayTags.h"

UGE_Lifesteal::UGE_Lifesteal()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo Mod;
	Mod.Attribute = UHealthSet::GetHealthAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat SBC;
	SBC.DataTag = MyGameplayTags::TAG_SetByCaller_Heal;
	FGameplayEffectModifierMagnitude MagSpec(SBC);
	Mod.ModifierMagnitude = MagSpec;
	Modifiers.Add(Mod);

	UAssetTagsGameplayEffectComponent& AssetComp =
		FindOrAddComponent<UAssetTagsGameplayEffectComponent>();
	FInheritedTagContainer Inh;
	Inh.Added.AddTag(MyGameplayTags::TAG_Effect_Heal);
	AssetComp.SetAndApplyAssetTagChanges(Inh);
}

UGE_ThornsDamage::UGE_ThornsDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// 反伤伤害写入 IncomingDamage（与正常伤害走同一管线，复用 PostExecute 减免逻辑）
	FGameplayModifierInfo Mod;
	Mod.Attribute = UHealthSet::GetIncomingDamageAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;

	FSetByCallerFloat SBC;
	SBC.DataTag = MyGameplayTags::TAG_SetByCaller_Damage;
	FGameplayEffectModifierMagnitude MagSpec(SBC);
	Mod.ModifierMagnitude = MagSpec;
	Modifiers.Add(Mod);

	// 关键：标记 NoReflect ，PostExecute 反伤逻辑里需要查这个 Tag 跳过
	UAssetTagsGameplayEffectComponent& AssetComp =
		FindOrAddComponent<UAssetTagsGameplayEffectComponent>();
	FInheritedTagContainer Inh;
	Inh.Added.AddTag(MyGameplayTags::TAG_Effect_Damage);
	Inh.Added.AddTag(MyGameplayTags::TAG_Effect_Damage_NoReflect);
	AssetComp.SetAndApplyAssetTagChanges(Inh);
}
