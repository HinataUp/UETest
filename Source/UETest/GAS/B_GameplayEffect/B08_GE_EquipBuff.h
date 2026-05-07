// B08 - Infinite GE + 三种移除方式：
//   方式1: RemoveActiveGameplayEffect(Handle)              —— 按 Handle
//   方式2: RemoveActiveGameplayEffectBySourceEffect(Class) —— 按 GE 类
//   方式3: RemoveActiveEffectsWithGrantedTags(Tags)        —— 按 Tag 查询

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "B08_GE_EquipBuff.generated.h"

UCLASS()
class UETEST_API UGE_EquipBuff : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_EquipBuff();
};
