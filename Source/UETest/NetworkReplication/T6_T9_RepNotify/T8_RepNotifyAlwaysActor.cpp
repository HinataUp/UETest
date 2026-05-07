// T8 - REPNOTIFY_Always 实现

#include "T8_RepNotifyAlwaysActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT8_RepNotifyAlwaysActor::AT8_RepNotifyAlwaysActor()
	: T8_AuraStacks(0)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT8_RepNotifyAlwaysActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 关键写法：DOREPLIFETIME_CONDITION_NOTIFY 第三参 COND_None ，第四参 REPNOTIFY_Always
	// 含义：所有连接都同步 + 值未变也触发 OnRep
	DOREPLIFETIME_CONDITION_NOTIFY(AT8_RepNotifyAlwaysActor,
		T8_AuraStacks, COND_None, REPNOTIFY_Always);
}

void AT8_RepNotifyAlwaysActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T8_TickTimer, this, &AT8_RepNotifyAlwaysActor::T8_ServerTickAura,
			2.0f, true);
	}
}

void AT8_RepNotifyAlwaysActor::T8_ServerTickAura()
{
	// 故意有时不变，演示 REPNOTIFY_Always 即便值不变也触发
	const int32 NewStacks = (T8_TickCount % 3 == 0) ? T8_AuraStacks : T8_TickCount;
	T8_ServerRefreshAura(NewStacks);
	++T8_TickCount;
}

void AT8_RepNotifyAlwaysActor::T8_ServerRefreshAura(int32 NewStacks)
{
	// 标准模式：Server 改值之前先存 OldValue
	const int32 OldStacks = T8_AuraStacks;
	T8_AuraStacks = NewStacks;

	// Server 手动调用 OnRep ，让 Authority 也走相同的表现逻辑
	// 这是 GAS 在应用 GameplayEffect 时的常见模式
	if (HasAuthority())
	{
		OnRep_T8_AuraStacks(OldStacks);
	}
}

void AT8_RepNotifyAlwaysActor::OnRep_T8_AuraStacks(int32 T8_OldStacks)
{
	// REPNOTIFY_Always 下，即便 NewStacks==OldStacks ，OnRep 也会被自动调用
	NET_LOG(TEXT("OnRep_AuraStacks Old=%d New=%d (Always)"),
		T8_OldStacks, T8_AuraStacks);
}
