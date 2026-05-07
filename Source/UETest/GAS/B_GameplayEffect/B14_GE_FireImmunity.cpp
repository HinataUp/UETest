#include "B14_GE_FireImmunity.h"
#include "GameplayEffectComponents/ImmunityGameplayEffectComponent.h"
#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"
#include "../A_Basics/A01_HealthSet.h"
#include "../00_Common/MyGameplayTags.h"

UGE_FireImmunity::UGE_FireImmunity()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	// 配置免疫 ：阻止 AssetTag 含 Effect.Damage.Fire 的 GE 应用到自己
	UImmunityGameplayEffectComponent& Comp =
		FindOrAddComponent<UImmunityGameplayEffectComponent>();
	FGameplayEffectQuery Query;
	Query.EffectTagQuery = FGameplayTagQuery::MakeQuery_MatchAnyTags(
		FGameplayTagContainer(MyGameplayTags::TAG_Effect_Damage_Fire));
	Comp.ImmunityQueries.Add(Query);
}

UGE_FireDamage::UGE_FireDamage()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo Mod;
	Mod.Attribute = UHealthSet::GetHealthAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;
	Mod.ModifierMagnitude = FScalableFloat(-30.f);
	Modifiers.Add(Mod);

	// 标记本 GE 是火焰伤害（用于免疫匹配）
	UAssetTagsGameplayEffectComponent& AssetComp =
		FindOrAddComponent<UAssetTagsGameplayEffectComponent>();
	FInheritedTagContainer Inh;
	Inh.Added.AddTag(MyGameplayTags::TAG_Effect_Damage);
	Inh.Added.AddTag(MyGameplayTags::TAG_Effect_Damage_Fire);
	AssetComp.SetAndApplyAssetTagChanges(Inh);
}
