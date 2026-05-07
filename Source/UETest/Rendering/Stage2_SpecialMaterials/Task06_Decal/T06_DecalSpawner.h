// T06 - 弹痕 Decal 生成器
//
// Decal 材质本身在编辑器中创建（Material Domain: Deferred Decal）。
// 运行时如何"在命中点生成 Decal + 5 秒淡出 + 自动销毁"则需要 C++ 控制。
//
// 用法：把本组件加到武器或 Pawn 上，外部传入命中信息后调用 SpawnBulletHole。

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "T06_DecalSpawner.generated.h"

class UMaterialInterface;
class UDecalComponent;

UCLASS(ClassGroup = (Render), meta = (BlueprintSpawnableComponent))
class UETEST_API UT06_DecalSpawner : public UActorComponent
{
	GENERATED_BODY()

public:
	UT06_DecalSpawner();

	// 在世界坐标 Location 生成弹痕，方向沿 -Normal（Decal Component -X 轴是投影方向）
	UFUNCTION(BlueprintCallable, Category = "Render|Decal")
	UDecalComponent* SpawnBulletHole(const FVector& Location, const FVector& Normal);

	// 在编辑器或派生类指定弹痕材质
	UPROPERTY(EditAnywhere, Category = "Render|Decal")
	TObjectPtr<UMaterialInterface> BulletDecalMaterial;

	// 弹痕大小（local extent ，不是 Scale）
	UPROPERTY(EditAnywhere, Category = "Render|Decal")
	FVector DecalSize = FVector(20.f, 20.f, 20.f);

	// 寿命（秒），到时自动销毁
	UPROPERTY(EditAnywhere, Category = "Render|Decal", meta = (ClampMin = "0.0"))
	float LifeSpan = 5.0f;

	// 淡出时间（秒），生命结束前的淡出长度
	UPROPERTY(EditAnywhere, Category = "Render|Decal", meta = (ClampMin = "0.0"))
	float FadeDuration = 1.0f;
};
