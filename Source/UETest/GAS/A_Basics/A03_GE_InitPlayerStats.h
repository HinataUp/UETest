// A03 - 用 GE 初始化属性（vs Native InitXxx 直接赋值）
//
// 两种方式对比：
//   方式A (Native)：HealthSet->InitHealth(100)  —— 不走 PreAttributeChange / PostExecute
//   方式B (GE)    ：ApplyGE(GE_InitPlayerStats) —— 走完整 GAS 管线，可由策划数据驱动
//
// 实战推荐：方式B（数据驱动），但要在 Post 中处理"初始化阶段"防护，
//   避免初始化期间血量瞬时穿过 0 触发死亡逻辑。

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "A03_GE_InitPlayerStats.generated.h"

UCLASS()
class UETEST_API UGE_InitPlayerStats : public UGameplayEffect
{
	GENERATED_BODY()
public:
	UGE_InitPlayerStats();
};
