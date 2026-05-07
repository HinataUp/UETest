// A04 - GameplayTag Native 注册（实现）

#include "MyGameplayTags.h"

namespace MyGameplayTags
{
	// Ability
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Skill_Fireball,         "Ability.Skill.Fireball");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Skill_Dash,             "Ability.Skill.Dash");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Skill_Combo,            "Ability.Skill.Combo");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Movement,               "Ability.Movement");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Movement_Channel,       "Ability.Movement.Channel");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Active_Dash,            "Ability.Active.Dash");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_FireType,               "Ability.FireType");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_IceType,                "Ability.IceType");

	// State
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_Dead,                     "State.Dead");
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_Stunned,                  "State.Stunned");
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_Immune,                   "State.Immune");
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_OnGround,                 "State.OnGround");
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_InRange,                  "State.InRange");
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_Burning,                  "State.Burning");
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_Slowed,                   "State.Slowed");
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_Equipped,                 "State.Equipped");
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_Combo_Ready,              "State.Combo.Ready");
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_Combat_NoCancel,          "State.Combat.NoCancel");
	UE_DEFINE_GAMEPLAY_TAG(TAG_State_Test,                     "State.Test");

	// Effect
	UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_Damage,                  "Effect.Damage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_Damage_Fire,             "Effect.Damage.Fire");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_Damage_NoReflect,        "Effect.Damage.NoReflect");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_Heal,                    "Effect.Heal");

	// Cooldown
	UE_DEFINE_GAMEPLAY_TAG(TAG_Cooldown_Skill_Fireball,        "Cooldown.Skill.Fireball");

	// GameplayCue
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Hit_Fire,           "GameplayCue.Hit.Fire");
	UE_DEFINE_GAMEPLAY_TAG(TAG_GameplayCue_Buff_Burning,       "GameplayCue.Buff.Burning");

	// SetByCaller
	UE_DEFINE_GAMEPLAY_TAG(TAG_SetByCaller_Damage,             "SetByCaller.Damage");
	UE_DEFINE_GAMEPLAY_TAG(TAG_SetByCaller_Heal,               "SetByCaller.Heal");

	// Input
	UE_DEFINE_GAMEPLAY_TAG(TAG_Input_Skill_Primary,            "Input.Skill.Primary");

	// Event
	UE_DEFINE_GAMEPLAY_TAG(TAG_Event_TriggerFireball,          "Event.TriggerFireball");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Montage_AttackHit,        "Event.Montage.AttackHit");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Combo_WindowOpen,         "Event.Combo.WindowOpen");
	UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Combo_WindowClose,        "Event.Combo.WindowClose");

	// Input Buffer
	UE_DEFINE_GAMEPLAY_TAG(TAG_Input_Pending_Attack,           "Input.Pending.Attack");
}
