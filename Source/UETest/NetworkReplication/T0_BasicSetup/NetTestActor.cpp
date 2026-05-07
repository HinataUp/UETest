// T0 - 网络同步测试基础 Actor 实现

#include "NetTestActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Common/NetTestLogMacros.h"

ANetTestActor::ANetTestActor()
{
	// 不需要 Tick 时关掉，节省性能
	PrimaryActorTick.bCanEverTick = false;

	// 关键：声明此 Actor 参与网络复制
	// 在构造函数里设最稳；运行时改用 SetReplicates(true)
	bReplicates = true;

	// 创建 RootComponent ，以 NT_ 前缀防止与 UE 内置命名冲突
	NT_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NT_MeshComp"));
	RootComponent = NT_MeshComp;

	// 用引擎自带的 Cube 资源，避免依赖外部资产
	// 写法解释：ConstructorHelpers::FObjectFinder 仅可在构造期使用，热重载安全
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(
		TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeAsset.Succeeded())
	{
		NT_MeshComp->SetStaticMesh(CubeAsset.Object);
	}
}

void ANetTestActor::BeginPlay()
{
	Super::BeginPlay();

	// 三端都会进 BeginPlay ，但身份不同
	// Server   ：ROLE_Authority
	// Client   ：ROLE_SimulatedProxy（无 Owner 时）
	NET_LOG(TEXT("BeginPlay"));
}

void ANetTestActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	NET_LOG(TEXT("EndPlay Reason=%d"), (int32)EndPlayReason);
	Super::EndPlay(EndPlayReason);
}
