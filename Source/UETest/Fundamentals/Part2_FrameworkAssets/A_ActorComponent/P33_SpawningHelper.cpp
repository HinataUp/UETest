#include "P33_SpawningHelper.h"
#include "Engine/World.h"
#include "../00_Common/FundP2LogMacros.h"

AActor* UP33_SpawningHelper::SpawnStandard(const UObject* WorldContext, TSubclassOf<AActor> Class, FTransform T)
{
	if (!WorldContext || !Class) { return nullptr; }
	UWorld* World = WorldContext->GetWorld();
	if (!World) { return nullptr; }

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AActor* Actor = World->SpawnActor<AActor>(Class, T, Params);
	P2_LOG("P33: SpawnStandard -> %s", Actor ? *Actor->GetName() : TEXT("null"));
	return Actor;
}

AActor* UP33_SpawningHelper::SpawnDeferredBegin(const UObject* WorldContext, TSubclassOf<AActor> Class, FTransform T)
{
	if (!WorldContext || !Class) { return nullptr; }
	UWorld* World = WorldContext->GetWorld();
	if (!World) { return nullptr; }

	// SpawnActorDeferred ：BeginPlay 不会立刻被调用 ；可以先设属性
	AActor* Actor = World->SpawnActorDeferred<AActor>(Class, T);
	P2_LOG("P33: SpawnDeferredBegin -> %s （未 FinishSpawning）",
		Actor ? *Actor->GetName() : TEXT("null"));
	return Actor;
}

void UP33_SpawningHelper::FinishDeferred(AActor* DeferredActor, FTransform T)
{
	if (!DeferredActor) { return; }
	// FinishSpawning ：触发 BeginPlay ；之前设的属性此时生效
	DeferredActor->FinishSpawning(T);
	P2_LOG("P33: FinishDeferred -> BeginPlay 已触发");
}
