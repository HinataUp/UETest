// T12 - Render Target 画布 ：运行时伤痕贴图
//
// 思路 ：
//   * 一张 RT (512x512, R8) 当作"伤痕遮罩"
//   * 角色材质里采样 RT ：白色显示伤痕 / 黑色显示正常
//   * 命中时调 PaintDamageAtUV ：用一个圆形笔刷材质画白色圆到 RT
//
// 性能注意 ：
//   * DrawMaterialToRenderTarget 实际上是一次全屏 pass 渲染，连续高频调用很贵
//   * 想批量绘制用 BeginDrawCanvasToRenderTarget + Canvas API 合并
//   * 不要每帧 ReadRenderTargetPixel ：会引起 GPU stall

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "T12_DamagePaintComponent.generated.h"

class UTextureRenderTarget2D;
class UMaterialInterface;
class UMaterialInstanceDynamic;

UCLASS(ClassGroup = (Render), meta = (BlueprintSpawnableComponent))
class UETEST_API UT12_DamagePaintComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UT12_DamagePaintComponent();

	// 在 RT 的 UV 坐标处画一个圆，模拟伤痕累积
	UFUNCTION(BlueprintCallable, Category = "Render|RT")
	void PaintDamageAtUV(FVector2D HitUV, float RadiusUV = 0.05f);

	// 清空所有伤痕
	UFUNCTION(BlueprintCallable, Category = "Render|RT")
	void ClearDamage();

	// 配置 ：RT 尺寸（构造期一次性创建）
	UPROPERTY(EditAnywhere, Category = "Render|RT", meta = (ClampMin = "64", ClampMax = "4096"))
	int32 RenderTargetSize = 512;

	// 笔刷材质（圆形径向渐变 ，输出 alpha = 圆心 1 -> 边缘 0）
	UPROPERTY(EditAnywhere, Category = "Render|RT")
	TObjectPtr<UMaterialInterface> BrushMaterial;

	// 拿 RT 给外部材质采样
	UFUNCTION(BlueprintPure, Category = "Render|RT")
	UTextureRenderTarget2D* GetDamageRT() const { return DamageRT; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UTextureRenderTarget2D> DamageRT;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> BrushMID;
};
