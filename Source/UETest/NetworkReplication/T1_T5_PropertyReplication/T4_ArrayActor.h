// T4 - TArray 容器复制
//
// 任务目标：观察 增 / 改 / 删 三种操作在网络上的表现
// 关键点：
//   * TArray 复制使用"全量比较 + 差异同步"，UE 内部按元素 Diff
//   * 大数组、高频改动是带宽热点
//   * 高频结构数组建议使用 FastArraySerializer（GAS 中常见，进阶话题，本 task 不展开）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T3_StructActor.h"      // 复用 FT3_CombatState 演示结构体数组
#include "T4_ArrayActor.generated.h"

UCLASS()
class UETEST_API AT4_ArrayActor : public AActor
{
	GENERATED_BODY()

public:
	AT4_ArrayActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<int32> T4_RepIntArray;

	// 结构体数组 ：成本明显高于基础类型数组
	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<FT3_CombatState> T4_RepStateArray;

private:
	FTimerHandle T4_TickTimer;
	int32 T4_TickCount = 0;
	void T4_ServerTickArray();
};
