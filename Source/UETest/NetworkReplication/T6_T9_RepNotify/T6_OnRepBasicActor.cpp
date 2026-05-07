// T6 - RepNotify 基础 实现

#include "T6_OnRepBasicActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT6_OnRepBasicActor::AT6_OnRepBasicActor()
	: T6_RepInt(0)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT6_OnRepBasicActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT6_OnRepBasicActor, T6_RepInt);
}

void AT6_OnRepBasicActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T6_TickTimer, this, &AT6_OnRepBasicActor::T6_ServerTickValue,
			2.0f, true);
	}
}

void AT6_OnRepBasicActor::T6_ServerTickValue()
{
	++T6_RepInt;
	NET_LOG(TEXT("ServerSet RepInt=%d (Authority will NOT trigger OnRep)"), T6_RepInt);
}

void AT6_OnRepBasicActor::OnRep_T6_RepInt()
{
	// 此回调只在"接收端"被自动调用：即 Simulated / Autonomous Proxy
	// Server 自己改自己的值不会触发；如需 Server 也走，参 T8
	NET_LOG(TEXT("OnRep_T6_RepInt fired, current=%d"), T6_RepInt);
}
