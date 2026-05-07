// P96 - Render Target + Canvas 绘制
//
// 与 Rendering 模块 T12 概念一致 ；本 Helper 是简洁版
// BeginDrawCanvasToRenderTarget → 用 UCanvas API 绘制 → EndDrawCanvasToRenderTarget
// 实战 ：小地图 / 动态贴花 / 运行时图标合成

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P96_RenderTargetHelper.generated.h"

class UTextureRenderTarget2D;

UCLASS()
class UETEST_API UP96_RenderTargetHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 创建 RT
	UFUNCTION(BlueprintCallable, Category = "P96|RT",
		meta = (WorldContext = "WorldContext"))
	static UTextureRenderTarget2D* CreateRT(
		const UObject* WorldContext, int32 Size = 512);

	// 用 Canvas 在 RT 上画一个圆
	UFUNCTION(BlueprintCallable, Category = "P96|RT",
		meta = (WorldContext = "WorldContext"))
	static void DrawCircleOnRT(
		const UObject* WorldContext, UTextureRenderTarget2D* RT,
		FVector2D ScreenPos, float Radius, FLinearColor Color);

	// 清空 RT 为指定颜色
	UFUNCTION(BlueprintCallable, Category = "P96|RT",
		meta = (WorldContext = "WorldContext"))
	static void ClearRT(
		const UObject* WorldContext, UTextureRenderTarget2D* RT,
		FLinearColor ClearColor = FLinearColor::Black);
};
