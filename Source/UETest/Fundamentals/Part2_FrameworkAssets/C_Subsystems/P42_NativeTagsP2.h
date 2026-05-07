// P42 - GameplayTag 系统（Part 2 用 ：与 GAS 模块 / Networking 模块的 Tag 不冲突）
//
// 复习 ：之前网络同步 / GAS 模块都已大量用 Native Tag ；此处只补充 P43 Message Channel 用的 Tag

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace P2_Tags
{
	// 用作 GameplayMessage Subsystem 的 Channel Tag
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Message_Damage_Dealt);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Message_Player_Joined);
}
