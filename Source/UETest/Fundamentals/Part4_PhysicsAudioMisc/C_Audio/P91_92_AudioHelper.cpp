#include "P91_92_AudioHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "../00_Common/FundP4LogMacros.h"

UAudioComponent* UP91_92_AudioHelper::Play3DSound(
	const UObject* WorldContext, USoundBase* Sound, FVector Location,
	float VolumeMultiplier, float PitchMultiplier,
	USoundAttenuation* Attenuation, USoundConcurrency* Concurrency)
{
	if (!WorldContext || !Sound) { return nullptr; }
	UWorld* World = WorldContext->GetWorld();
	if (!World) { return nullptr; }

	// SpawnSoundAtLocation ：返回 AudioComponent ，可后续 SetParameter
	UAudioComponent* AC = UGameplayStatics::SpawnSoundAtLocation(
		World, Sound, Location, FRotator::ZeroRotator,
		VolumeMultiplier, PitchMultiplier,
		/*StartTime=*/0.f, Attenuation, Concurrency);

	P4_LOG("P91-92: Play3D %s @ (%.0f,%.0f,%.0f)",
		*Sound->GetName(), Location.X, Location.Y, Location.Z);
	return AC;
}

void UP91_92_AudioHelper::Play2DSound(
	const UObject* WorldContext, USoundBase* Sound, float VolumeMultiplier)
{
	if (!WorldContext || !Sound) { return; }
	UGameplayStatics::PlaySound2D(WorldContext, Sound, VolumeMultiplier);
}

void UP91_92_AudioHelper::TriggerMetaSoundParameter(UAudioComponent* AC, FName ParamName)
{
	if (!AC) { return; }
	// MetaSound Trigger ：等价 Trigger Input 信号
	AC->SetTriggerParameter(ParamName);
}

void UP91_92_AudioHelper::SetMetaSoundFloat(UAudioComponent* AC, FName ParamName, float Value)
{
	if (!AC) { return; }
	AC->SetFloatParameter(ParamName, Value);
}
