// G36 - Loose Tag vs GE-Driven Tag
//
// Loose ：手动 AddLooseGameplayTag/RemoveLooseGameplayTag ，默认不复制
//          可用 EGameplayTagReplicationState::TagOnly 模式复制
// GE-Driven ：通过 GE.GrantedTags 自动管理 + 自动复制
//
// 实战经验 ：业务规则 Tag 用 Loose（如 InRange 这种本地判定）
//            状态 Tag 用 GE 驱动（自动管理生命周期 + 一致性）

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "G36_LooseVsGrantedTag.generated.h"

UCLASS()
class UETEST_API UGE_GrantTestTag : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_GrantTestTag();
};
