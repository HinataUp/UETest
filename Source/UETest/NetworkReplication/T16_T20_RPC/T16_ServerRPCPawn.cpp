// T16 - Server RPC 实现

#include "T16_ServerRPCPawn.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT16_ServerRPCPawn::AT16_ServerRPCPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	// Pawn 默认 bReplicates=true 且会被 Controller 设 Owner ，因此可以发 Server RPC
}

void AT16_ServerRPCPawn::BeginPlay()
{
	Super::BeginPlay();

	// 仅在"被 Possess 的 Client 端"运行 Tick ，模拟玩家按键
	// 注意：Listen Server 上的本地玩家也会进这个分支（它的 Pawn 在自机是 Authority+Autonomous 一体）
	if (GetLocalRole() == ROLE_AutonomousProxy || (HasAuthority() && IsLocallyControlled()))
	{
		GetWorldTimerManager().SetTimer(
			T16_TickTimer, this, &AT16_ServerRPCPawn::T16_ClientTickFire,
			1.0f, true);
	}
}

void AT16_ServerRPCPawn::T16_ClientTickFire()
{
	++T16_LocalCounter;
	// 同时发两路 RPC ，方便对比丢包后的差异（用 net.PktLoss 模拟）
	ServerSendReliable(T16_LocalCounter);
	ServerSendUnreliable(T16_LocalCounter);
	NET_LOG(TEXT("ClientFire local=%d"), T16_LocalCounter);
}

// _Implementation 是 RPC 真正执行体
// UHT 会根据 UFUNCTION 标记自动生成"分发函数"调用本实现
void AT16_ServerRPCPawn::ServerSendReliable_Implementation(int32 T16_Value)
{
	NET_LOG(TEXT("ServerRecv Reliable=%d"), T16_Value);
}

void AT16_ServerRPCPawn::ServerSendUnreliable_Implementation(int32 T16_Value)
{
	NET_LOG(TEXT("ServerRecv Unreliable=%d"), T16_Value);
}
