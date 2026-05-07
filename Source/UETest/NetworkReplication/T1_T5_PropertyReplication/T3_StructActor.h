// T3 - USTRUCT 复制
//
// 任务目标：理解结构体作为"原子单元"复制 ——
// 即使你只改了一个字段，整个结构体会作为一个整体被同步。
//
// 设计取舍：
//   * 多个相关字段放进一个结构体：网络包紧凑、变更一起到达，但任意字段变都触发整体重传
//   * 各字段独立 UPROPERTY(Replicated) ：每个字段独立判定是否变更，但需要为每个字段维护"是否脏"位
// 经验法则：相关性强、变更频率接近的字段放一起；独立变化的拆开。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T3_StructActor.generated.h"

// 注意：USTRUCT 必须 GENERATED_BODY()
// 内部 UPROPERTY 不需要 Replicated 标记，结构体随外层字段一起被序列化
USTRUCT(BlueprintType)
struct FT3_CombatState
{
	GENERATED_BODY()

	UPROPERTY() int32 Health   = 100;
	UPROPERTY() int32 Stamina  = 100;
	UPROPERTY() bool  bBlocking = false;
};

UCLASS()
class UETEST_API AT3_StructActor : public AActor
{
	GENERATED_BODY()

public:
	AT3_StructActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// 整个结构体作为复制单位
	UPROPERTY(Replicated, VisibleAnywhere)
	FT3_CombatState T3_CombatState;

private:
	FTimerHandle T3_TickTimer;
	void T3_ServerTickStruct();
};
