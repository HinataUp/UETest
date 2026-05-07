#include "P28_30_AsyncShowcase.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Async/Async.h"
#include "../00_Common/FundamentalsLogMacros.h"

AP28_30_AsyncShowcase::AP28_30_AsyncShowcase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AP28_30_AsyncShowcase::StartOneShotTimer(float DelaySeconds)
{
	UWorld* World = GetWorld();
	if (!World) { return; }

	World->GetTimerManager().SetTimer(
		OneShotHandle, this, &AP28_30_AsyncShowcase::OnTimerFired,
		FMath::Max(0.05f, DelaySeconds), /*bLoop=*/false);
}

void AP28_30_AsyncShowcase::StartRepeatTimer(float Interval)
{
	UWorld* World = GetWorld();
	if (!World) { return; }

	World->GetTimerManager().SetTimer(
		RepeatHandle, this, &AP28_30_AsyncShowcase::OnTimerFired,
		FMath::Max(0.05f, Interval), /*bLoop=*/true);
}

void AP28_30_AsyncShowcase::StopRepeatTimer()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RepeatHandle);
	}
}

void AP28_30_AsyncShowcase::OnTimerFired()
{
	++TimerFireCount;
	P1_LOG("P28: 定时器触发 ，累计 %d 次", TimerFireCount);
}

void AP28_30_AsyncShowcase::RunAsyncCompute(int32 Iterations)
{
	const int32 Loops = FMath::Max(1, Iterations);

	// P29 ：Async 在线程池执行
	// P30 ：后台线程 不能 直接访问 UObject ，必须 AsyncTask(GameThread) 调回主线程
	TWeakObjectPtr<AP28_30_AsyncShowcase> WeakThis(this);

	Async(EAsyncExecution::ThreadPool, [WeakThis, Loops]()
	{
		// 后台线程 ：纯 CPU 计算
		int32 Result = 0;
		for (int32 i = 1; i <= Loops; ++i)
		{
			Result += i;
		}

		// 调回主线程 ：访问 UObject 必须如此
		AsyncTask(ENamedThreads::GameThread, [WeakThis, Result]()
		{
			AP28_30_AsyncShowcase* StrongThis = WeakThis.Get();
			if (!StrongThis) { return; }   // Actor 可能已销毁

			// FScopeLock 演示 ：实际此处 GameThread 串行 ，不需要锁
			FScopeLock Lock(&StrongThis->ResultMutex);
			StrongThis->LastAsyncResult = Result;
			P1_LOG("P29-30: 异步计算完成 = %d", Result);
		});
	});
}

void AP28_30_AsyncShowcase::EndPlay(const EEndPlayReason::Type Reason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(OneShotHandle);
		World->GetTimerManager().ClearTimer(RepeatHandle);
	}
	Super::EndPlay(Reason);
}
