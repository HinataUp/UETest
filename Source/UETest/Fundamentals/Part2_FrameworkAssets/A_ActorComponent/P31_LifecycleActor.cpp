#include "P31_LifecycleActor.h"
#include "../00_Common/FundP2LogMacros.h"

AP31_LifecycleActor::AP31_LifecycleActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Ctor 阶段 ：不能访问 GetWorld()，但可以设默认值 / CreateDefaultSubobject
	StageFlags |= Stage_Ctor;
	P2_LOG("P31: Constructor 调用");
}

void AP31_LifecycleActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TouchStage(Stage_PostInit, TEXT("PostInitializeComponents"));
}

void AP31_LifecycleActor::BeginPlay()
{
	Super::BeginPlay();
	TouchStage(Stage_BeginPlay, TEXT("BeginPlay"));
}

void AP31_LifecycleActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if ((StageFlags & Stage_Tick) == 0)
	{
		// 只在第一次 Tick 标记 + 打日志（避免每帧刷屏）
		TouchStage(Stage_Tick, TEXT("Tick (first frame)"));
	}
}

void AP31_LifecycleActor::EndPlay(const EEndPlayReason::Type Reason)
{
	TouchStage(Stage_EndPlay, TEXT("EndPlay"));
	Super::EndPlay(Reason);
}

void AP31_LifecycleActor::Destroyed()
{
	TouchStage(Stage_Destroyed, TEXT("Destroyed"));
	Super::Destroyed();
}

void AP31_LifecycleActor::TouchStage(int32 Bit, const TCHAR* Name) const
{
	const_cast<AP31_LifecycleActor*>(this)->StageFlags |= Bit;
	P2_LOG("P31: %s | StageFlags=0x%02x", Name, StageFlags);
}
