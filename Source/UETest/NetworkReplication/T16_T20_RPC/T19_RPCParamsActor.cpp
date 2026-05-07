// T19 - RPC 参数类型 实现

#include "T19_RPCParamsActor.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT19_RPCParamsActor::AT19_RPCParamsActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT19_RPCParamsActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T19_TickTimer, this, &AT19_RPCParamsActor::T19_ServerInvoke,
			4.0f, true);
	}
}

void AT19_RPCParamsActor::T19_ServerInvoke()
{
	FT3_CombatState State;
	State.Health = 88;
	State.Stamina = 77;
	State.bBlocking = true;

	TArray<int32> Arr = { 1, 2, 3, 4 };

	// 用 this 作 Actor 引用 ：保证三端都能解析（Actor 自己一定存在）
	MulticastComplex(42, TEXT("Hello RPC"), State, this, Arr);
}

void AT19_RPCParamsActor::MulticastComplex_Implementation(
	int32 T19_IntVal,
	const FString& T19_StringVal,
	const FT3_CombatState& T19_StructVal,
	AActor* T19_ActorRef,
	const TArray<int32>& T19_ArrayVal)
{
	NET_LOG(TEXT("MultiComplex i=%d s=%s State(HP=%d ST=%d B=%d) Ref=%s ArrNum=%d"),
		T19_IntVal,
		*T19_StringVal,
		T19_StructVal.Health, T19_StructVal.Stamina, T19_StructVal.bBlocking ? 1 : 0,
		T19_ActorRef ? *T19_ActorRef->GetName() : TEXT("null"),
		T19_ArrayVal.Num());
}
