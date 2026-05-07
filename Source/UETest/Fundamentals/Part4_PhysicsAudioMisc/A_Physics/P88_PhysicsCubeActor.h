// P88 - Physics Simulation 基础
//
// SetSimulatePhysics(true) ：受重力 / 碰撞 / 力 影响
// AddForce        ：持续施力（每 Tick 调）
// AddImpulse      ：瞬时冲量（一次性）
// PhysicsMaterial ：Friction（摩擦）/ Restitution（弹性）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P88_PhysicsCubeActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class UETEST_API AP88_PhysicsCubeActor : public AActor
{
	GENERATED_BODY()

public:
	AP88_PhysicsCubeActor();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CubeMesh;

	UPROPERTY(EditAnywhere, Category = "P88|Physics", meta = (ClampMin = "0"))
	float ImpulseStrength = 50000.f;

	// 给方块一个瞬时冲量（如被弹飞）
	UFUNCTION(BlueprintCallable, Category = "P88|Physics")
	void ApplyImpulse(FVector Direction);

	// 持续施力（如风吹）
	UFUNCTION(BlueprintCallable, Category = "P88|Physics")
	void ApplyForce(FVector Force);
};
