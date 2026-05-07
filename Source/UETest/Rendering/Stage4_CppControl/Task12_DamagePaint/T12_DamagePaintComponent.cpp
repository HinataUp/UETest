#include "T12_DamagePaintComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "../../00_Common/RenderingLogMacros.h"

UT12_DamagePaintComponent::UT12_DamagePaintComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UT12_DamagePaintComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World) { return; }

	// 创建 RT （单通道 PF_R8 即可 ；想存彩色伤痕可改 PF_B8G8R8A8）
	DamageRT = UKismetRenderingLibrary::CreateRenderTarget2D(
		World, RenderTargetSize, RenderTargetSize,
		ETextureRenderTargetFormat::RTF_R8);

	if (DamageRT)
	{
		// 初始 ：清成黑色（无伤痕）
		UKismetRenderingLibrary::ClearRenderTarget2D(World, DamageRT, FLinearColor::Black);
	}

	// 笔刷 MID ：通过 MID 暴露中心 / 半径 参数
	if (BrushMaterial)
	{
		BrushMID = UMaterialInstanceDynamic::Create(BrushMaterial, this);
	}

	RT_LOG("DamagePaint 初始化 RT=%dx%d", RenderTargetSize, RenderTargetSize);
}

void UT12_DamagePaintComponent::PaintDamageAtUV(FVector2D HitUV, float RadiusUV)
{
	UWorld* World = GetWorld();
	if (!World || !DamageRT || !BrushMID) { return; }

	// 把命中 UV / 半径传给笔刷材质 ：
	// 笔刷在采样时检查 distance(uv, BrushCenter) < BrushRadius 就输出白色
	BrushMID->SetVectorParameterValue(TEXT("BrushCenter"),
		FLinearColor(HitUV.X, HitUV.Y, 0.f, 0.f));
	BrushMID->SetScalarParameterValue(TEXT("BrushRadius"),
		FMath::Clamp(RadiusUV, 0.001f, 0.5f));

	// 一次全屏 pass 把笔刷材质画到 RT 上 ：现有黑色不被清，新增的白色累积
	UKismetRenderingLibrary::DrawMaterialToRenderTarget(World, DamageRT, BrushMID);
}

void UT12_DamagePaintComponent::ClearDamage()
{
	if (UWorld* World = GetWorld())
	{
		UKismetRenderingLibrary::ClearRenderTarget2D(World, DamageRT, FLinearColor::Black);
	}
}
