// P10 - BlueprintFunctionLibrary + UBlueprintAsyncActionBase
//
// FunctionLibrary ：纯 static UFUNCTION 集合 ；BP 中可全局调用（无需 self）
// AsyncActionBase ：暴露多个动态多播委托 ；BP 中显示为多输出引脚的"异步节点"

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "P10_BlueprintLibrary.generated.h"

// ───────────────── BP Function Library ─────────────────
UCLASS()
class UETEST_API UP10_BlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// BlueprintPure ：BP 中纯函数节点（菱形）
	UFUNCTION(BlueprintPure, Category = "P10|Math")
	static int32 SquareInt(int32 In) { return In * In; }

	// BlueprintCallable ：有执行引脚 ；可改外部状态
	UFUNCTION(BlueprintCallable, Category = "P10|Util")
	static FString ConcatStrings(const TArray<FString>& Parts, const FString& Separator);
};

// ───────────────── BP AsyncAction Node ─────────────────
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP10_AsyncResultDelegate, int32, Result);

UCLASS()
class UETEST_API UP10_AsyncWaitTicks : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	// BP 中显示为节点 "Async Wait Ticks" ；输入参数 Ticks ；输出引脚 OnComplete / OnFail
	UFUNCTION(BlueprintCallable, Category = "P10|Async",
		meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContext"))
	static UP10_AsyncWaitTicks* AsyncWaitTicks(const UObject* WorldContext, int32 Ticks);

	UPROPERTY(BlueprintAssignable)
	FP10_AsyncResultDelegate OnComplete;

	UPROPERTY(BlueprintAssignable)
	FP10_AsyncResultDelegate OnFail;

	virtual void Activate() override;

private:
	int32 RemainingTicks = 0;
	TWeakObjectPtr<UWorld> CachedWorld;
	void TickStep();
};
