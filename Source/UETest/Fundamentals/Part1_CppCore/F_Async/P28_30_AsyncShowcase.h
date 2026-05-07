// P28-P30 - 异步 / 线程 / 定时器
//
// P28 FTimerManager      ：SetTimer / SetTimerForNextTick / Lambda
// P29 UE::Tasks + Async  ：UE5 新 Task 系统 / TFuture / ParallelFor
// P30 GameThread 调度    ：后台线程 -> AsyncTask(GameThread) ；FCriticalSection 保护

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P28_30_AsyncShowcase.generated.h"

UCLASS()
class UETEST_API AP28_30_AsyncShowcase : public AActor
{
	GENERATED_BODY()

public:
	AP28_30_AsyncShowcase();

	// P28 ：起一个 2s 后 单次回调 的定时器 ，回调中递增 TimerFireCount
	UFUNCTION(BlueprintCallable, Category = "P28|Timer")
	void StartOneShotTimer(float DelaySeconds = 2.f);

	// P28 ：起一个每 0.5s 重复触发 的定时器 ；外部调 StopRepeatTimer 停止
	UFUNCTION(BlueprintCallable, Category = "P28|Timer")
	void StartRepeatTimer(float Interval = 0.5f);

	UFUNCTION(BlueprintCallable, Category = "P28|Timer")
	void StopRepeatTimer();

	// P29 ：后台线程跑一段 work ，结束后 AsyncTask GameThread 把结果写回 LastAsyncResult
	UFUNCTION(BlueprintCallable, Category = "P29|Async")
	void RunAsyncCompute(int32 Iterations = 1000);

	UPROPERTY(VisibleAnywhere)
	int32 TimerFireCount = 0;

	UPROPERTY(VisibleAnywhere)
	int32 LastAsyncResult = 0;

protected:
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

private:
	FTimerHandle OneShotHandle;
	FTimerHandle RepeatHandle;

	void OnTimerFired();

	// P30 ：保护 LastAsyncResult 的写入（虽然只在 GameThread 写 ，但展示原语）
	FCriticalSection ResultMutex;
};
