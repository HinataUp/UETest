// P87 - Line / Sweep / Overlap Trace
//
// 三种空间查询 ：
//   LineTraceSingleByChannel  ：从 A 到 B 一条线 ；返回首个命中
//   SweepSingleByChannel      ：用 Shape（Sphere/Box/Capsule）扫一条线 ；返回首个命中
//   OverlapMultiByChannel     ：在某点放 Shape 检测重叠 ；返回所有
//
// 每种都有 _Multi 版本（返回所有命中）

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P87_TraceHelper.generated.h"

UCLASS()
class UETEST_API UP87_TraceHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Line Trace ：返回是否命中 + HitResult
	UFUNCTION(BlueprintCallable, Category = "P87|Trace",
		meta = (WorldContext = "WorldContext"))
	static bool LineTraceVisibility(
		const UObject* WorldContext, FVector Start, FVector End,
		FHitResult& OutHit, AActor* IgnoreActor = nullptr,
		bool bDrawDebug = false);

	// Sphere Sweep ：用球形扫描
	UFUNCTION(BlueprintCallable, Category = "P87|Trace",
		meta = (WorldContext = "WorldContext"))
	static bool SphereSweep(
		const UObject* WorldContext, FVector Start, FVector End,
		float Radius, FHitResult& OutHit,
		AActor* IgnoreActor = nullptr,
		bool bDrawDebug = false);

	// Sphere Overlap ：在某点找半径内所有 Pawn
	UFUNCTION(BlueprintCallable, Category = "P87|Trace",
		meta = (WorldContext = "WorldContext"))
	static int32 SphereOverlapPawns(
		const UObject* WorldContext, FVector Origin, float Radius,
		TArray<AActor*>& OutActors,
		bool bDrawDebug = false);
};
