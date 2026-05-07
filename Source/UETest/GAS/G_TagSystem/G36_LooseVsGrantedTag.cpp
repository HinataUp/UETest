#include "G36_LooseVsGrantedTag.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "../00_Common/MyGameplayTags.h"

UGE_GrantTestTag::UGE_GrantTestTag()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	UTargetTagsGameplayEffectComponent& Comp =
		FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer Inh;
	Inh.Added.AddTag(MyGameplayTags::TAG_State_Test);
	Comp.SetAndApplyTargetTagChanges(Inh);
}
