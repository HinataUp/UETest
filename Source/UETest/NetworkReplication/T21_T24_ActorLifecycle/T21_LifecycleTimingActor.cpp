// T21 - Actor 复制生命周期 实现

#include "T21_LifecycleTimingActor.h"
#include "Net/UnrealNetwork.h"
#include "../Common/NetTestLogMacros.h"

AT21_LifecycleTimingActor::AT21_LifecycleTimingActor()
	: T21_InitialValue(0)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT21_LifecycleTimingActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT21_LifecycleTimingActor, T21_InitialValue);
}

void AT21_LifecycleTimingActor::BeginPlay()
{
	Super::BeginPlay();

	// Authority 端在 BeginPlay 之前就把 InitialValue 改了
	// 实际项目里 Server 在 SpawnActorDeferred + FinishSpawning 期间设置
	if (HasAuthority())
	{
		T21_InitialValue = 999;
	}

	// 三端都打 InitialValue ：Client 端首次进入 BeginPlay 时也应能读到 999
	NET_LOG(TEXT("BeginPlay InitialValue=%d"), T21_InitialValue);
}

void AT21_LifecycleTimingActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	NET_LOG(TEXT("EndPlay Reason=%d"), (int32)EndPlayReason);
	Super::EndPlay(EndPlayReason);
}
