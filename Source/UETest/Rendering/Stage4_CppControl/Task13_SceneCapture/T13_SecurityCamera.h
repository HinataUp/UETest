// T13 - SceneCapture 安全摄像头 + 画中画
//
// SceneCapture2D 完整地多渲染一遍场景到 RT ，是 UE 中最贵的渲染操作之一。
// 控制成本三件事 ：
//   1. CaptureEveryFrame=false ；用 Timer 间隔捕获
//   2. ShowFlags 关闭无用通道 ：Atmosphere / Fog / SSR / Bloom 等
//   3. RT 尺寸不要过大 ：256x256 通常够用

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T13_SecurityCamera.generated.h"

class USceneCaptureComponent2D;
class UTextureRenderTarget2D;

UCLASS()
class UETEST_API AT13_SecurityCamera : public AActor
{
	GENERATED_BODY()

public:
	AT13_SecurityCamera();

	// 捕获间隔（秒）；越小越实时但越贵
	UPROPERTY(EditAnywhere, Category = "Render|SceneCapture", meta = (ClampMin = "0.05"))
	float CaptureInterval = 0.5f;

	// RT 尺寸 ；监控屏小窗口 256 即可
	UPROPERTY(EditAnywhere, Category = "Render|SceneCapture", meta = (ClampMin = "64", ClampMax = "2048"))
	int32 RenderTargetSize = 256;

	// 故障概率 0-1 ：每次捕获时按此概率跳过（模拟信号干扰）
	UPROPERTY(EditAnywhere, Category = "Render|SceneCapture", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float GlitchChance = 0.1f;

	// 暴露 RT 给监控屏材质采样
	UFUNCTION(BlueprintPure, Category = "Render|SceneCapture")
	UTextureRenderTarget2D* GetCaptureRT() const { return CaptureRT; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneCaptureComponent2D> CaptureComp;

private:
	UPROPERTY()
	TObjectPtr<UTextureRenderTarget2D> CaptureRT;

	FTimerHandle CaptureTimer;
	void DoCapture();
};
