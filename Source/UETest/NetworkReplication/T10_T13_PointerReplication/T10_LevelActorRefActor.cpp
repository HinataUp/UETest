// T10 - 复制关卡中已存在的 Actor 指针 实现

#include "T10_LevelActorRefActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT10_LevelActorRefActor::AT10_LevelActorRefActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT10_LevelActorRefActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT10_LevelActorRefActor, T10_TargetActor);
}

void AT10_LevelActorRefActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		// 延迟 2 秒后 Server 将场景中的目标赋给 Replicated 字段
		GetWorldTimerManager().SetTimer(
			T10_TickTimer, this, &AT10_LevelActorRefActor::T10_ServerAssignTarget,
			2.0f, false);
	}
}

void AT10_LevelActorRefActor::T10_ServerAssignTarget()
{
	// 关卡 Actor 在三端都存在（默认 bNetLoadOnClient=true ），所以引用可以被解析
	T10_TargetActor = T10_TargetInLevel;
	NET_LOG(TEXT("ServerAssign Target=%s"),
		T10_TargetActor ? *T10_TargetActor->GetName() : TEXT("null"));
}

void AT10_LevelActorRefActor::OnRep_T10_TargetActor()
{
	// 关卡 Actor 引用通常一次解析成功
	NET_LOG(TEXT("OnRep_TargetActor=%s"),
		T10_TargetActor ? *T10_TargetActor->GetName() : TEXT("null"));
}
