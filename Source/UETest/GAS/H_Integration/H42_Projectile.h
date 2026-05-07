// H42 - 投射物：Server 权威 + Client 本地视觉球
//
// AProjectile 本身仅 Server 创建（权威）
// Client 端按键瞬间用 SpawnVisualOnly 创建一个本地视觉球（不参与命中）
// 命中由 Server 判定，命中表现通过 GameplayCue 广播

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "H42_Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class UETEST_API AGASProjectile : public AActor
{
	GENERATED_BODY()
public:
	AGASProjectile();

	// 用于 Client 本地视觉球 ：构造时关闭碰撞 / 复制
	void MarkAsVisualOnly();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> Movement;

	bool bIsVisualOnly = false;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& Sweep);
};
