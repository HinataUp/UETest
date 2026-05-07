#include "B08_GE_EquipBuff.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "../A_Basics/A01_HealthSet.h"
#include "../00_Common/MyGameplayTags.h"

UGE_EquipBuff::UGE_EquipBuff()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	FGameplayModifierInfo Mod;
	Mod.Attribute = UHealthSet::GetMaxHealthAttribute();
	Mod.ModifierOp = EGameplayModOp::Additive;
	Mod.ModifierMagnitude = FScalableFloat(50.f);
	Modifiers.Add(Mod);

	// GrantedTags ：UE5.3+ 用 UTargetTagsGameplayEffectComponent
	UTargetTagsGameplayEffectComponent& TagsComp =
		FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer Inh;
	Inh.Added.AddTag(MyGameplayTags::TAG_State_Equipped);
	TagsComp.SetAndApplyTargetTagChanges(Inh);
}
