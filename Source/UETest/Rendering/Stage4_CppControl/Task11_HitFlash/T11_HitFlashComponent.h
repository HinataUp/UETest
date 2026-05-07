// T11 - Dynamic Material Instance ：受击闪白系统
//
// 标准 C++↔材质 桥梁 ：
//   * 角色材质里预留 HitFlashIntensity / HitFlashColor 参数
//   * BeginPlay 时一次性 CreateDynamicMaterialInstance 缓存到 UPROPERTY
//   * TriggerFlash 时启动 Timer ，0.15s 内 lerp Intensity 1->0
//
// 关键约束 ：
//   * MID 必须 UPROPERTY 持有 ，否则 GC 回收
//   * CreateDynamicMaterialInstance 不能在 Tick 里调（每次都分配新 GPU 资源）
//   * Replicate 的应该是触发数值（bHitFlashFiring），不是 MID 指针

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "T11_HitFlashComponent.generated.h"

class UMeshComponent;
class UMaterialInstanceDynamic;

UCLASS(ClassGroup = (Render), meta = (BlueprintSpawnableComponent))
class UETEST_API UT11_HitFlashComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UT11_HitFlashComponent();

	// 触发一次闪白 ：参数 0..1 强度，颜色支持自定义
	UFUNCTION(BlueprintCallable, Category = "Render|HitFlash")
	void TriggerFlash(float Intensity = 1.0f, FLinearColor Color = FLinearColor::White);

	// 闪白持续时间
	UPROPERTY(EditAnywhere, Category = "Render|HitFlash", meta = (ClampMin = "0.05"))
	float FlashDuration = 0.15f;

	// 在哪个 Slot Index 创建 MID（0 通常是身体材质）
	UPROPERTY(EditAnywhere, Category = "Render|HitFlash")
	int32 MaterialSlotIndex = 0;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	// 缓存 MID ：UPROPERTY 防 GC
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> CachedMID;

private:
	UMeshComponent* FindOwnerMesh() const;

	// Tick-less 衰减 ：用 Timer 驱动
	FTimerHandle FlashTimer;
	float FlashElapsed = 0.f;
	float FlashStartIntensity = 0.f;
	FLinearColor FlashColor = FLinearColor::White;

	// 高频更新优化路径 ：缓存参数 Index ，避免每次按名查找
	int32 IntensityParamIndex = INDEX_NONE;
	int32 ColorParamIndex = INDEX_NONE;

	void TickFlashUpdate();
};
