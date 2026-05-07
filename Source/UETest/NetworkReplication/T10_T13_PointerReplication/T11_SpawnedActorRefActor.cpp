// T11 - 复制 Server 运行时 Spawn 的 Actor 指针 实现

#include "T11_SpawnedActorRefActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "../Common/NetTestLogMacros.h"

AT11_SpawnedActorRefActor::AT11_SpawnedActorRefActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT11_SpawnedActorRefActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT11_SpawnedActorRefActor, T11_SpawnedRef);
}

void AT11_SpawnedActorRefActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T11_TickTimer, this, &AT11_SpawnedActorRefActor::T11_ServerSpawnAndRef,
			3.0f, false);
	}
}

void AT11_SpawnedActorRefActor::T11_ServerSpawnAndRef()
{
	if (!T11_SpawnClass) { return; }

	// 1) Server 端 Spawn 一个全新 Actor —— 必须 bReplicates 才能复制到 Client
	// SpawnActor 默认采用同帧 Spawn ；如要延后初始化可用 SpawnActorDeferred
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* NewOne = GetWorld()->SpawnActor<AActor>(
		T11_SpawnClass,
		GetActorLocation() + FVector(200.f, 0.f, 0.f),
		FRotator::ZeroRotator,
		SpawnParams);

	// 2) 立刻把指针赋给 Replicated 字段
	// 此时 Client 可能还没收到 NewOne 的复制，OnRep 第一次可能拿到 nullptr
	T11_SpawnedRef = NewOne;
	NET_LOG(TEXT("ServerSpawn => %s"),
		T11_SpawnedRef ? *T11_SpawnedRef->GetName() : TEXT("null"));
}

void AT11_SpawnedActorRefActor::OnRep_T11_SpawnedRef()
{
	// 工程做法：第一次 null 不是 bug ，UE 会等被引用 Actor 到达后再次触发
	if (T11_SpawnedRef)
	{
		NET_LOG(TEXT("OnRep_SpawnedRef Resolved=%s"), *T11_SpawnedRef->GetName());
	}
	else
	{
		NET_LOG(TEXT("OnRep_SpawnedRef = nullptr (Unmapped, will retry)"));
	}
}
