// T9 - 同帧多次修改属性的合并行为 实现

#include "T9_BatchedChangesActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT9_BatchedChangesActor::AT9_BatchedChangesActor()
	: T9_HitCount(0)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT9_BatchedChangesActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT9_BatchedChangesActor, T9_HitCount);
	DOREPLIFETIME(AT9_BatchedChangesActor, T9_HitHistory);
}

void AT9_BatchedChangesActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T9_TickTimer, this, &AT9_BatchedChangesActor::T9_ServerBurstHits,
			3.0f, true);
	}
}

void AT9_BatchedChangesActor::T9_ServerBurstHits()
{
	// 模拟"一帧内连续受击 5 次"
	for (int32 i = 0; i < 5; ++i)
	{
		++T9_HitCount;
		T9_HitHistory.Add(T9_HitCount);
		// Server 自身能看到每次变化
		NET_LOG(TEXT("ServerHit step=%d Count=%d"), i, T9_HitCount);
	}
	// 提示：Client 端 OnRep 只会被调用 1 次，且 Count == 5 的累计值
	// 但 T9_HitHistory 里能看到完整 5 个元素 —— 这是用数组保留事件序列的标准做法
}

void AT9_BatchedChangesActor::OnRep_T9_HitCount()
{
	NET_LOG(TEXT("OnRep_HitCount=%d HistorySize=%d (一帧内连续 ++ 后只触发一次)"),
		T9_HitCount, T9_HitHistory.Num());
}
