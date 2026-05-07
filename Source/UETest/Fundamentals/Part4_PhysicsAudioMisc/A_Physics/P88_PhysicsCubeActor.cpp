#include "P88_PhysicsCubeActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AP88_PhysicsCubeActor::AP88_PhysicsCubeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = CubeMesh;

	// 启用物理 ：受重力 / 力 / 碰撞
	CubeMesh->SetSimulatePhysics(true);
	CubeMesh->SetMobility(EComponentMobility::Movable);
	CubeMesh->SetCollisionProfileName(TEXT("PhysicsActor"));

	// 默认 Cube 资源（避免外部依赖）
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded())
	{
		CubeMesh->SetStaticMesh(Cube.Object);
	}
}

void AP88_PhysicsCubeActor::ApplyImpulse(FVector Direction)
{
	if (!CubeMesh) { return; }
	// AddImpulse 第三参 bVelChange=true ：忽略质量直接改速度（爆炸式）
	CubeMesh->AddImpulse(Direction.GetSafeNormal() * ImpulseStrength,
		NAME_None, /*bVelChange=*/false);
}

void AP88_PhysicsCubeActor::ApplyForce(FVector Force)
{
	if (!CubeMesh) { return; }
	// 持续施力 ：通常每 Tick 调用 ；本帧 DeltaTime 由物理引擎处理
	CubeMesh->AddForce(Force);
}
