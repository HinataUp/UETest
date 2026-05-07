// D27 - 跨目标效果：吸血 + 反伤
//
// ExecCalc 的 OutputModifier 只能改 Target ；要给 Source 加血需要在 PostExecute 中
// 二次 ApplyGameplayEffectToSource 。本 task 提供两个 GE：
//
// GE_Heal       ：在 Target 的 PostExecute 中通过 SetByCaller 应用到 Source
// GE_ThornsDamage：反伤；AssetTag 含 NoReflect ，避免无限循环

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "D27_GE_Lifesteal.generated.h"

UCLASS()
class UETEST_API UGE_Lifesteal : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_Lifesteal();
};

UCLASS()
class UETEST_API UGE_ThornsDamage : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_ThornsDamage();
};
