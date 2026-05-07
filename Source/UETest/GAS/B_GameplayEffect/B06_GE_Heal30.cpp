#include "B06_GE_Heal30.h"
#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"
#include "../A_Basics/A01_HealthSet.h"
#include "../00_Common/MyGameplayTags.h"

UGE_Heal30::UGE_Heal30()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo Mod;
	Mod.Attribute = UHealthSet::GetHealthAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;
	Mod.ModifierMagnitude = FScalableFloat(30.f);
	Modifiers.Add(Mod);

	// AssetTags ：用于按 Tag 查询 / 过滤（如反伤判断）
	UAssetTagsGameplayEffectComponent& AssetTagsComp =
		FindOrAddComponent<UAssetTagsGameplayEffectComponent>();
	FInheritedTagContainer Inh;
	Inh.Added.AddTag(MyGameplayTags::TAG_Effect_Heal);
	AssetTagsComp.SetAndApplyAssetTagChanges(Inh);
}
