// T13 - 复制 Component 指针 实现

#include "T13_ComponentRefActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT13_ComponentRefActor::AT13_ComponentRefActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	T13_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("T13_MeshComp"));
	RootComponent = T13_MeshComp;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(
		TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeAsset.Succeeded())
	{
		T13_MeshComp->SetStaticMesh(CubeAsset.Object);
	}
}

void AT13_ComponentRefActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT13_ComponentRefActor, T13_RepMeshRef);
}

void AT13_ComponentRefActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T13_TickTimer, this, &AT13_ComponentRefActor::T13_ServerAssignSelfMesh,
			2.0f, false);
	}
}

void AT13_ComponentRefActor::T13_ServerAssignSelfMesh()
{
	// 自身 DefaultSubobject 的 NetGUID 在两端一致，因此 Client 能成功解析
	T13_RepMeshRef = T13_MeshComp;
	NET_LOG(TEXT("ServerAssign MeshRef=%s"),
		T13_RepMeshRef ? *T13_RepMeshRef->GetName() : TEXT("null"));
}

void AT13_ComponentRefActor::OnRep_T13_RepMeshRef()
{
	NET_LOG(TEXT("OnRep_RepMeshRef=%s"),
		T13_RepMeshRef ? *T13_RepMeshRef->GetName() : TEXT("null"));
}
