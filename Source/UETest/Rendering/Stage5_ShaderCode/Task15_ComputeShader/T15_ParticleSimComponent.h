// T15 - Compute Shader 调度组件
//
// 提供 BP / 蓝图友好接口 ：调一次 KickoffSim 就把粒子数据上传 + dispatch CS 一次。
// 实际项目里 ：粒子位置回读到 Niagara DataInterface ，或绑给 ISM 实例变换。

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "T15_ParticleSimShader.h"
#include "T15_ParticleSimComponent.generated.h"

UCLASS(ClassGroup = (Render), meta = (BlueprintSpawnableComponent))
class UETEST_API UT15_ParticleSimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UT15_ParticleSimComponent();

	// 设置初始粒子（通常在 BeginPlay 调一次）
	UFUNCTION(BlueprintCallable, Category = "Render|Compute")
	void InitParticles(int32 NumParticles);

	// 推一帧模拟 ：通常在 Tick 中调
	UFUNCTION(BlueprintCallable, Category = "Render|Compute")
	void Simulate(float DeltaSeconds);

	UPROPERTY(EditAnywhere, Category = "Render|Compute", meta = (ClampMin = "0.0"))
	float Gravity = 980.f;

protected:
	virtual void BeginPlay() override;

	// CPU 端镜像 ：仅用于初始化 ；Simulate 之后数据保留在 GPU
	TArray<FT15_ParticleData> CPUParticles;
	int32 CachedParticleCount = 0;
};
