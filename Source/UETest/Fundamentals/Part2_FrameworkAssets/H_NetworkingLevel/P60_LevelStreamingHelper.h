// P60 - World Partition + Level Streaming
//
// World Partition (UE5)  ：自动流式加载 ；编辑器配置 ；不需要代码（但仍可代码控制 Data Layer）
// 手动 Streaming         ：UGameplayStatics::LoadStreamLevel / UnloadStreamLevel
// ServerTravel           ：World->ServerTravel("/Game/Maps/NewMap") + bSeamlessTravel

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P60_LevelStreamingHelper.generated.h"

UCLASS()
class UETEST_API UP60_LevelStreamingHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 加载子关卡（按名字 / 软引用）
	UFUNCTION(BlueprintCallable, Category = "P60|Streaming",
		meta = (WorldContext = "WorldContext"))
	static void LoadStreamLevelByName(
		const UObject* WorldContext, FName LevelName,
		bool bMakeVisibleAfterLoad = true, bool bShouldBlock = false);

	// 卸载
	UFUNCTION(BlueprintCallable, Category = "P60|Streaming",
		meta = (WorldContext = "WorldContext"))
	static void UnloadStreamLevelByName(
		const UObject* WorldContext, FName LevelName, bool bShouldBlock = false);

	// Server Travel ：用于在线模式切关卡（带玩家）
	UFUNCTION(BlueprintCallable, Category = "P60|Travel",
		meta = (WorldContext = "WorldContext"))
	static void ServerTravelToMap(const UObject* WorldContext, const FString& MapPath);
};
