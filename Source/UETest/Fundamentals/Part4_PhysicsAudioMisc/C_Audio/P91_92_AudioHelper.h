// P91 + P92 - 音频 Helper
//
// P91 MetaSounds ：UE5 新音频系统 ；UMetaSoundSource 资产 + Trigger / Float 参数
//   * SpawnSoundAtLocation 会返回 UAudioComponent ；AC->SetTriggerParameter / SetFloatParameter 控制
//
// P92 SoundCue + Attenuation + Concurrency ：
//   * USoundAttenuation ：空间化衰减（远近声音大小）
//   * USoundConcurrency ：同类音效最大同时播放数
//
// 本 Helper 提供 BP 友好接口 ：3D 播放 / 2D 播放 / 控制 MetaSound 参数

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P91_92_AudioHelper.generated.h"

class USoundBase;
class USoundAttenuation;
class USoundConcurrency;
class UAudioComponent;

UCLASS()
class UETEST_API UP91_92_AudioHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 3D 播放（带衰减）；返回 AudioComponent 句柄 ，可后续控参
	UFUNCTION(BlueprintCallable, Category = "P91-92|Audio",
		meta = (WorldContext = "WorldContext"))
	static UAudioComponent* Play3DSound(
		const UObject* WorldContext, USoundBase* Sound, FVector Location,
		float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f,
		USoundAttenuation* Attenuation = nullptr,
		USoundConcurrency* Concurrency = nullptr);

	// 2D 播放（UI 音效）
	UFUNCTION(BlueprintCallable, Category = "P91-92|Audio",
		meta = (WorldContext = "WorldContext"))
	static void Play2DSound(
		const UObject* WorldContext, USoundBase* Sound,
		float VolumeMultiplier = 1.f);

	// MetaSound 触发参数 ：等价 Trigger Input 信号
	UFUNCTION(BlueprintCallable, Category = "P91-92|Audio")
	static void TriggerMetaSoundParameter(UAudioComponent* AC, FName ParamName);

	// MetaSound Float 参数 ：等价 Float Input
	UFUNCTION(BlueprintCallable, Category = "P91-92|Audio")
	static void SetMetaSoundFloat(UAudioComponent* AC, FName ParamName, float Value);
};
