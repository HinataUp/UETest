// P33 - Actor Spawning 三种方式
//
// SpawnActor          ：标准 ；BeginPlay 在 Spawn 内立即执行
// SpawnActorDeferred  ：BeginPlay 前可设属性 ；FinishSpawning 才触发 BeginPlay
// FActorSpawnParameters ：Owner / Instigator / SpawnCollisionHandling

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P33_SpawningHelper.generated.h"

UCLASS()
class UETEST_API UP33_SpawningHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 用 SpawnActor 标准方式生成
	UFUNCTION(BlueprintCallable, Category = "P33", meta = (WorldContext = "WorldContext"))
	static AActor* SpawnStandard(const UObject* WorldContext, TSubclassOf<AActor> Class, FTransform T);

	// 用 SpawnActorDeferred 生成 ：返回未 FinishSpawning 的 Actor
	// 调用方设完属性后 ，必须自己调 FinishSpawning(T)
	UFUNCTION(BlueprintCallable, Category = "P33", meta = (WorldContext = "WorldContext"))
	static AActor* SpawnDeferredBegin(const UObject* WorldContext, TSubclassOf<AActor> Class, FTransform T);

	// 完成 Deferred Spawn ：触发 BeginPlay
	UFUNCTION(BlueprintCallable, Category = "P33")
	static void FinishDeferred(AActor* DeferredActor, FTransform T);
};
