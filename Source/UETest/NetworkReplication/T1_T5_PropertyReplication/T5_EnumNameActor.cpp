// T5 - 枚举 / FName 复制 实现

#include "T5_EnumNameActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT5_EnumNameActor::AT5_EnumNameActor()
	: T5_Phase(ET5_CombatPhase::Idle)
	, T5_StateName(TEXT("Idle"))
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT5_EnumNameActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT5_EnumNameActor, T5_Phase);
	DOREPLIFETIME(AT5_EnumNameActor, T5_StateName);
}

void AT5_EnumNameActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T5_TickTimer, this, &AT5_EnumNameActor::T5_ServerTickPhase,
			2.0f, true);
	}
}

void AT5_EnumNameActor::T5_ServerTickPhase()
{
	// 循环切换状态机
	const ET5_CombatPhase Phases[] = {
		ET5_CombatPhase::Idle,
		ET5_CombatPhase::Attacking,
		ET5_CombatPhase::Blocking,
		ET5_CombatPhase::Stunned,
	};
	T5_Phase = Phases[T5_TickCount % UE_ARRAY_COUNT(Phases)];
	T5_StateName = FName(*FString::Printf(TEXT("Phase_%d"), (int32)T5_Phase));
	++T5_TickCount;

	// UEnum::GetValueAsString 用于把枚举值反射成可读字符串，日志友好
	NET_LOG(TEXT("Phase=%s Name=%s"),
		*UEnum::GetValueAsString(T5_Phase),
		*T5_StateName.ToString());
}
