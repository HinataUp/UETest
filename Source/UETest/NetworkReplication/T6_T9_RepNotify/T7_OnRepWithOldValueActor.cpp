// T7 - 带 OldValue 的 OnRep 实现

#include "T7_OnRepWithOldValueActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT7_OnRepWithOldValueActor::AT7_OnRepWithOldValueActor()
	: T7_Health(100)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT7_OnRepWithOldValueActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT7_OnRepWithOldValueActor, T7_Health);
}

void AT7_OnRepWithOldValueActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T7_TickTimer, this, &AT7_OnRepWithOldValueActor::T7_ServerTickHealth,
			2.0f, true);
	}
}

void AT7_OnRepWithOldValueActor::T7_ServerTickHealth()
{
	// 前若干次扣血，后若干次加血，方便观察 OnRep 的 OldValue
	if (T7_TickCount < 4)
	{
		T7_Health -= 20;
	}
	else
	{
		T7_Health += 20;
	}
	T7_Health = FMath::Clamp(T7_Health, 0, 200);
	++T7_TickCount;

	NET_LOG(TEXT("ServerSet Health=%d"), T7_Health);
}

void AT7_OnRepWithOldValueActor::OnRep_T7_Health(int32 T7_OldHealth)
{
	// 用差值判断"受伤还是恢复"，做不同表现
	const int32 Delta = T7_Health - T7_OldHealth;
	const TCHAR* DirStr = (Delta < 0) ? TEXT("Decreased") : TEXT("Increased/Same");
	NET_LOG(TEXT("OnRep_Health Old=%d New=%d (%s)"),
		T7_OldHealth, T7_Health, DirStr);
}
