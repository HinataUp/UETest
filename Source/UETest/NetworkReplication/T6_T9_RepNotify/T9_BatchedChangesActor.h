// T9 - 一帧内连续修改属性的合并行为
//
// 任务目标：理解"属性复制是状态同步，不是事件同步"
//   Server 同帧内 0->1->2->3->4->5，Client 只会收到 1 次 OnRep ，值为 5
//
// 启示：
//   * 不能用 Replicated int 同步"事件计数"。需要事件语义请用 Multicast RPC 或 FastArraySerializer
//   * 同步的是"截至本次复制时刻的状态"，不保证中间过程

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T9_BatchedChangesActor.generated.h"

UCLASS()
class UETEST_API AT9_BatchedChangesActor : public AActor
{
	GENERATED_BODY()

public:
	AT9_BatchedChangesActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_T9_HitCount, VisibleAnywhere)
	int32 T9_HitCount;

	// 用 Array 模拟"事件历史"，避免漏掉中间过程
	UPROPERTY(Replicated, VisibleAnywhere)
	TArray<int32> T9_HitHistory;

	UFUNCTION()
	void OnRep_T9_HitCount();

private:
	FTimerHandle T9_TickTimer;
	void T9_ServerBurstHits();
};
