// A04 - GameplayTag Native 注册
//
// Native 注册的优点：编译期确定、IDE 跳转可达、无字符串拼写错误
// 用法：UE_DECLARE_GAMEPLAY_TAG_EXTERN 在 .h 声明，UE_DEFINE_GAMEPLAY_TAG 在 .cpp 注册
//
// 命名约定（项目共识）：
//   Ability.Skill.<技能名>          — 技能身份
//   State.<状态>                    — 角色状态
//   Effect.<分类>.<子类>            — GE 资产标签
//   Cooldown.Skill.<技能名>         — 冷却 Tag
//   GameplayCue.<分类>.<子类>       — Cue Tag （前缀 GameplayCue. 必须）
//   SetByCaller.<参数名>            — SetByCaller 参数 Tag
//   Input.<输入>                    — Input 绑定 Tag
//   Event.<事件>                    — GameplayEvent Tag

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace MyGameplayTags
{
	// Ability 身份
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Skill_Fireball);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Skill_Dash);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Skill_Combo);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Movement);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Movement_Channel);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Active_Dash);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_FireType);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_IceType);

	// State
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Dead);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Stunned);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Immune);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_OnGround);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_InRange);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Burning);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Slowed);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Equipped);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Combo_Ready);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Combat_NoCancel);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Test);

	// Effect 资产 Tag
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Damage_Fire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Damage_NoReflect);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_Heal);

	// Cooldown
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Skill_Fireball);

	// GameplayCue
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Hit_Fire);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_GameplayCue_Buff_Burning);

	// SetByCaller
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SetByCaller_Damage);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_SetByCaller_Heal);

	// Input
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Skill_Primary);

	// Event
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_TriggerFireball);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Montage_AttackHit);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Combo_WindowOpen);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Combo_WindowClose);

	// Input Buffer
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Input_Pending_Attack);
}
