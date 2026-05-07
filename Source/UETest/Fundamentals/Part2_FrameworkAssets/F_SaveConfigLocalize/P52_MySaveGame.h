// P52 - USaveGame ：UGameplayStatics::SaveGameToSlot / LoadGameFromSlot
//
// 关键 ：UPROPERTY(SaveGame) 标记的字段才会序列化到 .sav 文件
// 文件位置 ：<Project>/Saved/SaveGames/<SlotName>.sav

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P52_MySaveGame.generated.h"

USTRUCT(BlueprintType)
struct FP52_InventoryItem
{
	GENERATED_BODY()

	UPROPERTY(SaveGame) FName ItemId;
	UPROPERTY(SaveGame) int32 Count = 0;
};

UCLASS()
class UETEST_API UP52_MySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame) FString PlayerName;
	UPROPERTY(SaveGame) int32   Level = 1;
	UPROPERTY(SaveGame) TArray<FP52_InventoryItem> Inventory;
};

UCLASS()
class UETEST_API UP52_SaveGameHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "P52|Save")
	static bool SaveToSlot(UP52_MySaveGame* SaveData, const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "P52|Save")
	static UP52_MySaveGame* LoadFromSlot(const FString& SlotName);
};
