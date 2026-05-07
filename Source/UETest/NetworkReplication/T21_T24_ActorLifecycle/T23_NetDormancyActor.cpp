// T23 - NetDormancy 实现

#include "T23_NetDormancyActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT23_NetDormancyActor::AT23_NetDormancyActor()
	: T23_RepValue(0)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	// 写法1：构造期设置初始休眠
	NetDormancy = DORM_Initial;

	// 写法2 演示（注释）：DORM_DormantAll 适合"已经稳定的 Actor"，业务上可随时再 Flush
	// NetDormancy = DORM_DormantAll;
}

void AT23_NetDormancyActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT23_NetDormancyActor, T23_RepValue);
}

void AT23_NetDormancyActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		// 每秒 Server 改值；但因为休眠，Client 看不到变化
		GetWorldTimerManager().SetTimer(
			T23_TickTimer, this, &AT23_NetDormancyActor::T23_ServerTickValue,
			1.0f, true);

		// 6 秒后 Flush 一次 ，唤醒一次复制；之后再回休眠
		GetWorldTimerManager().SetTimer(
			T23_WakeTimer, this, &AT23_NetDormancyActor::T23_ServerFlushOnce,
			6.0f, false);
	}
}

void AT23_NetDormancyActor::T23_ServerTickValue()
{
	++T23_RepValue;
	NET_LOG(TEXT("ServerTick Val=%d (Dormant)"), T23_RepValue);
}

void AT23_NetDormancyActor::T23_ServerFlushOnce()
{
	// 触发本 Actor 一次复制周期；脏字段会被同步到 Client
	// 然后 Server 会按 NetDormancy 配置再次进入休眠
	FlushNetDormancy();
	NET_LOG(TEXT("ServerFlushNetDormancy now Val=%d"), T23_RepValue);

	// 工程做法：长期不需要复制时改回 DORM_DormantAll
	SetNetDormancy(DORM_DormantAll);
}
