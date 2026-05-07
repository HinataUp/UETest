#include "P96_RenderTargetHelper.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/Canvas.h"
#include "../00_Common/FundP4LogMacros.h"

UTextureRenderTarget2D* UP96_RenderTargetHelper::CreateRT(
	const UObject* WorldContext, int32 Size)
{
	if (!WorldContext) { return nullptr; }
	UWorld* World = WorldContext->GetWorld();
	if (!World) { return nullptr; }

	UTextureRenderTarget2D* RT = UKismetRenderingLibrary::CreateRenderTarget2D(
		World, Size, Size, ETextureRenderTargetFormat::RTF_RGBA8);
	if (RT)
	{
		UKismetRenderingLibrary::ClearRenderTarget2D(World, RT, FLinearColor::Black);
	}
	P4_LOG("P96: 创建 RT %dx%d", Size, Size);
	return RT;
}

void UP96_RenderTargetHelper::DrawCircleOnRT(
	const UObject* WorldContext, UTextureRenderTarget2D* RT,
	FVector2D ScreenPos, float Radius, FLinearColor Color)
{
	if (!WorldContext || !RT) { return; }

	UCanvas* Canvas = nullptr;
	FVector2D Size;
	FDrawToRenderTargetContext Ctx;
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(
		WorldContext, RT, Canvas, Size, Ctx);

	if (Canvas)
	{
		// Canvas API ：用 N 段直线模拟圆 ；正式项目可用 K2_DrawTexture / DrawMaterial
		const int32 Sides = 32;
		const float Step = 2.f * PI / Sides;
		for (int32 i = 0; i < Sides; ++i)
		{
			const FVector2D A = ScreenPos + FVector2D(FMath::Cos(i * Step), FMath::Sin(i * Step)) * Radius;
			const FVector2D B = ScreenPos + FVector2D(FMath::Cos((i + 1) * Step), FMath::Sin((i + 1) * Step)) * Radius;
			Canvas->K2_DrawLine(A, B, /*Thickness=*/2.f, Color);
		}
	}

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(WorldContext, Ctx);
}

void UP96_RenderTargetHelper::ClearRT(
	const UObject* WorldContext, UTextureRenderTarget2D* RT, FLinearColor ClearColor)
{
	if (!WorldContext || !RT) { return; }
	UKismetRenderingLibrary::ClearRenderTarget2D(WorldContext, RT, ClearColor);
}
