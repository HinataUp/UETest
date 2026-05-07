// T15 - PreReplication 动态条件 实现

#include "T15_PreReplicationActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT15_PreReplicationActor::AT15_PreReplicationActor()
	: T15_ConditionalField(0)
	, T15_bShouldReplicate(false)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT15_PreReplicationActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT15_PreReplicationActor, T15_ConditionalField);
}

void AT15_PreReplicationActor::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	// 写法解释：第三参为 bool ：true 表示这次复制周期内允许该字段同步；false 屏蔽
	// 老 API ：DOREPLIFETIME_ACTIVE_OVERRIDE （兼容 UE4 / UE5 全版本）
	// 新 API ：DOREPLIFETIME_ACTIVE_OVERRIDE_FAST （UE5 PushModel 优化版，自行替换）
	DOREPLIFETIME_ACTIVE_OVERRIDE(AT15_PreReplicationActor,
		T15_ConditionalField, T15_bShouldReplicate);
}

void AT15_PreReplicationActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		// Value 每 1 秒 ++
		GetWorldTimerManager().SetTimer(
			T15_TickValueTimer, this, &AT15_PreReplicationActor::T15_ServerTickValue,
			1.0f, true);

		// 开关每 5 秒翻转
		GetWorldTimerManager().SetTimer(
			T15_ToggleTimer, this, &AT15_PreReplicationActor::T15_ServerToggleSwitch,
			5.0f, true);
	}
}

void AT15_PreReplicationActor::T15_ServerTickValue()
{
	++T15_ConditionalField;
	NET_LOG(TEXT("ServerTick Value=%d (Replicating=%d)"),
		T15_ConditionalField, T15_bShouldReplicate ? 1 : 0);
}

void AT15_PreReplicationActor::T15_ServerToggleSwitch()
{
	T15_bShouldReplicate = !T15_bShouldReplicate;
	NET_LOG(TEXT("ServerToggle bShouldReplicate=%d"),
		T15_bShouldReplicate ? 1 : 0);
}
