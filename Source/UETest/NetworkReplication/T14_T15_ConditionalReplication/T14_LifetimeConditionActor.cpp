// T14 - 五种 DOREPLIFETIME_CONDITION 实现

#include "T14_LifetimeConditionActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT14_LifetimeConditionActor::AT14_LifetimeConditionActor()
	: T14_RepOwnerOnly(0)
	, T14_RepSkipOwner(0)
	, T14_RepSimulatedOnly(0)
	, T14_RepAutonomousOnly(0)
	, T14_RepInitialOnly(0)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT14_LifetimeConditionActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 五种条件复制声明
	DOREPLIFETIME_CONDITION(AT14_LifetimeConditionActor, T14_RepOwnerOnly,      COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AT14_LifetimeConditionActor, T14_RepSkipOwner,      COND_SkipOwner);
	DOREPLIFETIME_CONDITION(AT14_LifetimeConditionActor, T14_RepSimulatedOnly,  COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(AT14_LifetimeConditionActor, T14_RepAutonomousOnly, COND_AutonomousOnly);
	DOREPLIFETIME_CONDITION(AT14_LifetimeConditionActor, T14_RepInitialOnly,    COND_InitialOnly);
}

void AT14_LifetimeConditionActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		// InitialOnly 字段需在 Actor 首次复制前赋值（这里在 BeginPlay 早期赋）
		T14_RepInitialOnly = 12345;

		GetWorldTimerManager().SetTimer(
			T14_TickTimer, this, &AT14_LifetimeConditionActor::T14_ServerTickAll,
			2.0f, true);
	}
}

void AT14_LifetimeConditionActor::T14_ServerTickAll()
{
	// 全部 ++ ，但 Client 端各字段可见性受 COND 影响
	++T14_RepOwnerOnly;
	++T14_RepSkipOwner;
	++T14_RepSimulatedOnly;
	++T14_RepAutonomousOnly;
	++T14_RepInitialOnly;       // 注意：InitialOnly 字段后续变更，Client 不会再收到

	NET_LOG(TEXT("Server O=%d Sk=%d Sim=%d Au=%d Init=%d"),
		T14_RepOwnerOnly,
		T14_RepSkipOwner,
		T14_RepSimulatedOnly,
		T14_RepAutonomousOnly,
		T14_RepInitialOnly);
}
