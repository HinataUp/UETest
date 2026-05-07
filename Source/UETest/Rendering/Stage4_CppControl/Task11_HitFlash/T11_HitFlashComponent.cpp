#include "T11_HitFlashComponent.h"
#include "Components/MeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "../../00_Common/RenderingLogMacros.h"

UT11_HitFlashComponent::UT11_HitFlashComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UMeshComponent* UT11_HitFlashComponent::FindOwnerMesh() const
{
	const AActor* Owner = GetOwner();
	return Owner ? Owner->FindComponentByClass<UMeshComponent>() : nullptr;
}

void UT11_HitFlashComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UMeshComponent* Mesh = FindOwnerMesh())
	{
		// 一次性创建 MID ：BeginPlay ，不在 Tick 里
		CachedMID = Mesh->CreateDynamicMaterialInstance(MaterialSlotIndex);

		if (CachedMID)
		{
			// 缓存参数 Index ：高频更新时用 SetScalarParameterByIndex 比 ByName 更快
			IntensityParamIndex = CachedMID->InitializeScalarParameterAndGetIndex(
				TEXT("HitFlashIntensity"), 0.f);
			ColorParamIndex = CachedMID->InitializeVectorParameterAndGetIndex(
				TEXT("HitFlashColor"), FLinearColor::White);
		}
	}
	else
	{
		RT_LOG("HitFlash: Owner 上找不到 MeshComponent");
	}
}

void UT11_HitFlashComponent::EndPlay(const EEndPlayReason::Type Reason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(FlashTimer);
	}
	Super::EndPlay(Reason);
}

void UT11_HitFlashComponent::TriggerFlash(float Intensity, FLinearColor Color)
{
	if (!CachedMID) { return; }

	FlashStartIntensity = FMath::Clamp(Intensity, 0.f, 1.f);
	FlashColor = Color;
	FlashElapsed = 0.f;

	// 用 ByIndex 路径设颜色 ：仅一次设置 ，比 ByName 快
	if (ColorParamIndex != INDEX_NONE)
	{
		CachedMID->SetVectorParameterByIndex(ColorParamIndex, FlashColor);
	}

	UWorld* World = GetWorld();
	if (!World) { return; }

	// 30Hz Tick 衰减（足够手感，不浪费）
	World->GetTimerManager().SetTimer(
		FlashTimer, this, &UT11_HitFlashComponent::TickFlashUpdate,
		1.f / 30.f, true);
}

void UT11_HitFlashComponent::TickFlashUpdate()
{
	FlashElapsed += 1.f / 30.f;
	const float Alpha = FMath::Clamp(FlashElapsed / FlashDuration, 0.f, 1.f);
	const float Current = FMath::Lerp(FlashStartIntensity, 0.f, Alpha);

	if (CachedMID && IntensityParamIndex != INDEX_NONE)
	{
		CachedMID->SetScalarParameterByIndex(IntensityParamIndex, Current);
	}

	if (Alpha >= 1.f)
	{
		// 收尾 ：彻底归零并停 Timer
		if (CachedMID && IntensityParamIndex != INDEX_NONE)
		{
			CachedMID->SetScalarParameterByIndex(IntensityParamIndex, 0.f);
		}
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(FlashTimer);
		}
	}
}
