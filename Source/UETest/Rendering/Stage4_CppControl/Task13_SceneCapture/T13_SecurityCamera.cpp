#include "T13_SecurityCamera.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "TimerManager.h"
#include "../../00_Common/RenderingLogMacros.h"

AT13_SecurityCamera::AT13_SecurityCamera()
{
	PrimaryActorTick.bCanEverTick = false;

	CaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CaptureComp"));
	RootComponent = CaptureComp;

	// 关键性能配置 ：
	CaptureComp->bCaptureEveryFrame = false;     // 不要每帧捕获 ！
	CaptureComp->bCaptureOnMovement = false;     // 也不要因移动捕获
	CaptureComp->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;

	// ShowFlags 减负 ：监控画面不需要这些昂贵效果
	CaptureComp->ShowFlags.SetAtmosphere(false);
	CaptureComp->ShowFlags.SetFog(false);
	CaptureComp->ShowFlags.SetScreenSpaceReflections(false);
	CaptureComp->ShowFlags.SetBloom(false);
	CaptureComp->ShowFlags.SetMotionBlur(false);
	CaptureComp->ShowFlags.SetEyeAdaptation(false);
}

void AT13_SecurityCamera::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World) { return; }

	// 创建 RT
	CaptureRT = UKismetRenderingLibrary::CreateRenderTarget2D(
		World, RenderTargetSize, RenderTargetSize,
		ETextureRenderTargetFormat::RTF_RGBA8);
	CaptureComp->TextureTarget = CaptureRT;

	// 启动捕获 Timer
	World->GetTimerManager().SetTimer(
		CaptureTimer, this, &AT13_SecurityCamera::DoCapture,
		CaptureInterval, /*bLoop=*/true);

	RT_LOG("SecurityCamera 启动 ：每 %.2fs 捕获一次", CaptureInterval);
}

void AT13_SecurityCamera::EndPlay(const EEndPlayReason::Type Reason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CaptureTimer);
	}
	Super::EndPlay(Reason);
}

void AT13_SecurityCamera::DoCapture()
{
	if (!CaptureComp) { return; }

	// 故障模拟 ：随机跳过本次捕获 ，画面停留在上一帧
	if (FMath::FRand() < GlitchChance)
	{
		RT_LOG("SecurityCamera 故障 ：跳过本次捕获");
		return;
	}

	CaptureComp->CaptureScene();
}
