#include "T04_WeatherMPCHelper.h"
#include "Engine/World.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "../../00_Common/RenderingLogMacros.h"

namespace
{
	UMaterialParameterCollectionInstance* GetMPCI(const UObject* Ctx, UMaterialParameterCollection* MPC)
	{
		if (!Ctx || !MPC) { return nullptr; }
		const UWorld* World = Ctx->GetWorld();
		return World ? World->GetParameterCollectionInstance(MPC) : nullptr;
	}
}

void UT04_WeatherMPCHelper::SetWetness(const UObject* WorldContext, UMaterialParameterCollection* MPC, float Wetness)
{
	if (auto* Inst = GetMPCI(WorldContext, MPC))
	{
		Inst->SetScalarParameterValue(TEXT("Wetness"), FMath::Clamp(Wetness, 0.f, 1.f));
		RT_LOG("MPC Wetness=%.2f", Wetness);
	}
}

void UT04_WeatherMPCHelper::SetRainIntensity(const UObject* WorldContext, UMaterialParameterCollection* MPC, float Intensity)
{
	if (auto* Inst = GetMPCI(WorldContext, MPC))
	{
		Inst->SetScalarParameterValue(TEXT("RainIntensity"), FMath::Clamp(Intensity, 0.f, 1.f));
	}
}

void UT04_WeatherMPCHelper::SetWindDirection(const UObject* WorldContext, UMaterialParameterCollection* MPC, FVector Direction)
{
	if (auto* Inst = GetMPCI(WorldContext, MPC))
	{
		const FVector Norm = Direction.GetSafeNormal();
		Inst->SetVectorParameterValue(TEXT("WindDirection"), FLinearColor(Norm.X, Norm.Y, Norm.Z, 0.f));
	}
}
