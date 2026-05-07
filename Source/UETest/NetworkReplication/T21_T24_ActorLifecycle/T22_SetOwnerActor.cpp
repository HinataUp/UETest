// T22 - SetOwner 对 Autonomous 判定的影响 实现

#include "T22_SetOwnerActor.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "../Common/NetTestLogMacros.h"

AT22_SetOwnerActor::AT22_SetOwnerActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT22_SetOwnerActor::BeginPlay()
{
	Super::BeginPlay();

	// 5 秒后 Server 把 Player1 的 PC 设为 Owner
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T22_AssignTimer, this, &AT22_SetOwnerActor::T22_ServerAssignOwner,
			5.0f, false);
	}

	// 三端每秒打 Role ，前 5 秒应是 Simulated ，第 6 秒后 Player1 看到自己变 Autonomous
	GetWorldTimerManager().SetTimer(
		T22_LogTimer, this, &AT22_SetOwnerActor::T22_LogRoleEachSecond,
		1.0f, true);
}

void AT22_SetOwnerActor::T22_ServerAssignOwner()
{
	// 取第一个 PlayerController（多人时可遍历 PlayerControllerIterator）
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		SetOwner(PC);
		NET_LOG(TEXT("ServerSetOwner -> %s"), *PC->GetName());
	}
}

void AT22_SetOwnerActor::T22_LogRoleEachSecond()
{
	// LocalRole 由 UE 网络层根据 Owner 链动态计算
	NET_LOG(TEXT("CurrentRole tick"));
}
