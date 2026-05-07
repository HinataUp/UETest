#include "C20_GA_TagFiveSet.h"
#include "AbilitySystemComponent.h"
#include "../00_Common/MyGameplayTags.h"
#include "../00_Common/GASLogMacros.h"

UGA_Dash::UGA_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// 五件套
	ActivationOwnedTags.AddTag(MyGameplayTags::TAG_Ability_Active_Dash);
	ActivationRequiredTags.AddTag(MyGameplayTags::TAG_State_OnGround);
	ActivationBlockedTags.AddTag(MyGameplayTags::TAG_State_Stunned);

	AbilityTags.AddTag(MyGameplayTags::TAG_Ability_Skill_Dash);
	AbilityTags.AddTag(MyGameplayTags::TAG_Ability_Movement);  // 用于被 Fireball Cancel
}

void UGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle H, const FGameplayAbilityActorInfo* AI, const FGameplayAbilityActivationInfo I, const FGameplayEventData*)
{
	if (!CommitAbility(H, AI, I)) { EndAbility(H, AI, I, true, true); return; }
	GAS_LOG("【GA-Tag】Dash 激活成功");
	EndAbility(H, AI, I, true, false);
}

UGA_FireballSkill::UGA_FireballSkill()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// Cancel ：激活时取消所有带 Ability.Movement 的 GA（如 Dash、Channel）
	CancelAbilitiesWithTag.AddTag(MyGameplayTags::TAG_Ability_Movement);

	AbilityTags.AddTag(MyGameplayTags::TAG_Ability_Skill_Fireball);
}

void UGA_FireballSkill::ActivateAbility(const FGameplayAbilitySpecHandle H, const FGameplayAbilityActorInfo* AI, const FGameplayAbilityActivationInfo I, const FGameplayEventData*)
{
	if (!CommitAbility(H, AI, I)) { EndAbility(H, AI, I, true, true); return; }
	GAS_LOG("【GA-Tag】Fireball 激活，已 Cancel 所有移动型 GA");
	EndAbility(H, AI, I, true, false);
}
