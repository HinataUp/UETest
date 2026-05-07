// T17 - Client RPC 实现

#include "T17_ClientRPCPawn.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT17_ClientRPCPawn::AT17_ClientRPCPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT17_ClientRPCPawn::BeginPlay()
{
	Super::BeginPlay();

	// 仅 Server 启动 Timer ；Client RPC 一定从 Server 发起
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T17_TickTimer, this, &AT17_ClientRPCPawn::T17_ServerTickNotifyOwner,
			3.0f, true);
	}
}

void AT17_ClientRPCPawn::T17_ServerTickNotifyOwner()
{
	// 仅 Owner 客户端会执行 _Implementation
	const FString Msg = FString::Printf(
		TEXT("Hello at %.1f"), GetWorld()->GetTimeSeconds());
	ClientShowMessage(Msg);
}

void AT17_ClientRPCPawn::ClientShowMessage_Implementation(const FString& T17_Msg)
{
	NET_LOG(TEXT("ClientRecv Msg=%s"), *T17_Msg);
}
