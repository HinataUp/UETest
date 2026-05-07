// T18 - Multicast RPC 实现

#include "T18_MulticastRPCActor.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT18_MulticastRPCActor::AT18_MulticastRPCActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT18_MulticastRPCActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T18_TickTimer, this, &AT18_MulticastRPCActor::T18_ServerBroadcastVFX,
			3.0f, true);
	}
}

void AT18_MulticastRPCActor::T18_ServerBroadcastVFX()
{
	// Server 触发 Multicast ；三端都会进入 _Implementation
	MulticastPlayVFX(GetActorLocation());
}

void AT18_MulticastRPCActor::MulticastPlayVFX_Implementation(FVector T18_Location)
{
	// 实际项目里这里会播 NiagaraSystem / 音效
	NET_LOG(TEXT("MulticastVFX at (%.1f,%.1f,%.1f)"),
		T18_Location.X, T18_Location.Y, T18_Location.Z);
}
