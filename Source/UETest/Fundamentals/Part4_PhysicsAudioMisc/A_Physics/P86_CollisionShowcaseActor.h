// P86 - Collision Channel + Response 矩阵
//
// Project Settings → Collision ：
//   * Object Channels ：自定义 ECollisionChannel ；如 "Projectile" / "Interactable"
//   * Trace Channels  ：用于 LineTrace / Sweep
//   * Profiles        ：预设响应矩阵 ：BlockAll / OverlapAll / Custom...
//
// 代码层 ：
//   SetCollisionProfileName    ：用预设
//   SetCollisionObjectType     ：自身属于哪个 Channel
//   SetCollisionResponseToChannel ：对某 Channel 的响应（Block/Overlap/Ignore）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P86_CollisionShowcaseActor.generated.h"

class USphereComponent;

UCLASS()
class UETEST_API AP86_CollisionShowcaseActor : public AActor
{
	GENERATED_BODY()

public:
	AP86_CollisionShowcaseActor();

	// SphereComponent 既能 Block 又能广播 Overlap ：经典做法
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComp;

protected:
	virtual void BeginPlay() override;

	// OnComponentHit ：阻挡时触发（Block 响应）
	UFUNCTION()
	void OnSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// OnComponentBeginOverlap ：穿透时触发（Overlap 响应）
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
};
