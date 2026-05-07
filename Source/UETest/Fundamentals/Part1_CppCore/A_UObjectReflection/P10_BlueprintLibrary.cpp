#include "P10_BlueprintLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "../00_Common/FundamentalsLogMacros.h"

FString UP10_BlueprintLibrary::ConcatStrings(const TArray<FString>& Parts, const FString& Separator)
{
	return FString::Join(Parts, *Separator);
}

UP10_AsyncWaitTicks* UP10_AsyncWaitTicks::AsyncWaitTicks(const UObject* WorldContext, int32 Ticks)
{
	UP10_AsyncWaitTicks* Action = NewObject<UP10_AsyncWaitTicks>();
	Action->RemainingTicks = FMath::Max(1, Ticks);
	if (WorldContext)
	{
		Action->CachedWorld = WorldContext->GetWorld();
	}
	// AsyncActionBase 必须 RegisterWithGameInstance 才能持续存在 ；
	// BP 中触发节点时引擎自动调 Activate
	return Action;
}

void UP10_AsyncWaitTicks::Activate()
{
	if (!CachedWorld.IsValid())
	{
		OnFail.Broadcast(-1);
		SetReadyToDestroy();
		return;
	}
	TickStep();
}

void UP10_AsyncWaitTicks::TickStep()
{
	if (!CachedWorld.IsValid())
	{
		OnFail.Broadcast(-1);
		SetReadyToDestroy();
		return;
	}

	if (--RemainingTicks <= 0)
	{
		OnComplete.Broadcast(0);
		SetReadyToDestroy();   // 关键 ：用完即销毁
		return;
	}

	// 用 Tick 模拟"每帧推进一次"
	FTimerHandle Handle;
	CachedWorld->GetTimerManager().SetTimerForNextTick(
		FTimerDelegate::CreateUObject(this, &UP10_AsyncWaitTicks::TickStep));
}
