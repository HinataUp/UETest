// P21-P24 - 容器与字符串
//
// P21 TArray              ：动态数组 + 算法
// P22 TMap / TSet / TMultiMap : 哈希容器
// P23 FString / FName / FText  ：三种字符串
// P24 TOptional / TTuple / TVariant : C++ 模式对应类型

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P21_24_ContainersLibrary.generated.h"

UCLASS()
class UETEST_API UP21_24_ContainersLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ───────── P21 TArray 算法 ─────────
	UFUNCTION(BlueprintCallable, Category = "P21|Array")
	static void DemoTArray(TArray<int32>& InOutArray);

	// ───────── P22 TMap 用法 ─────────
	UFUNCTION(BlueprintCallable, Category = "P22|Map")
	static int32 DemoTMap(TMap<FName, int32>& InOutMap, FName Key);

	// ───────── P23 FString / FName / FText 转换 ─────────
	UFUNCTION(BlueprintCallable, Category = "P23|String")
	static FString DemoStringTypes(FName InName, FText& OutText);
};
