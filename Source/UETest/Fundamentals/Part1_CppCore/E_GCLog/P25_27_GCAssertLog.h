// P25-P27 - GC / 断言 / 日志
//
// P25 GC ：UObject 强引用 vs WeakPtr ；AddToRoot / RemoveFromRoot ；FGCObject 见 P16
// P26 断言 ：check / ensure / verify
// P27 自定义 LogCategory + UE_LOG / UE_LOGFMT

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P25_27_GCAssertLog.generated.h"

// 自定义日志分类（按章节分独立 category 便于过滤）
DECLARE_LOG_CATEGORY_EXTERN(LogP1_GCAssert, Log, All);

UCLASS()
class UETEST_API UP25_27_GCAssertLog : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ─── P25 ：GC 演示 ─────
	// 强制一次完整 GC
	UFUNCTION(BlueprintCallable, Category = "P25|GC")
	static void ForceFullGC();

	// 把 Obj 加到 GC Root（永远不被回收 ，除非 RemoveFromRoot）
	UFUNCTION(BlueprintCallable, Category = "P25|GC")
	static void AddToRoot(UObject* Obj);

	UFUNCTION(BlueprintCallable, Category = "P25|GC")
	static void RemoveFromRoot(UObject* Obj);

	// ─── P26 ：断言 演示 ─────
	// 调时 Value <= 0 会触发 ensureMsgf ；Output Log 看 callstack（不崩溃）
	UFUNCTION(BlueprintCallable, Category = "P26|Assert")
	static void DemoEnsure(int32 Value);

	// ─── P27 ：日志 ─────
	UFUNCTION(BlueprintCallable, Category = "P27|Log")
	static void DemoLog(const FString& Message);
};
