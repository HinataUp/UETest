// T3 - USTRUCT 复制 实现

#include "T3_StructActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT3_StructActor::AT3_StructActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT3_StructActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT3_StructActor, T3_CombatState);
}

void AT3_StructActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T3_TickTimer, this, &AT3_StructActor::T3_ServerTickStruct,
			2.0f, true);
	}
}

void AT3_StructActor::T3_ServerTickStruct()
{
	// 只改一个字段，但整个结构体会重传
	T3_CombatState.Health -= 5;
	if (T3_CombatState.Health <= 0)
	{
		T3_CombatState.Health = 100;
	}

	NET_LOG(TEXT("Struct HP=%d ST=%d Block=%d"),
		T3_CombatState.Health,
		T3_CombatState.Stamina,
		T3_CombatState.bBlocking ? 1 : 0);
}
