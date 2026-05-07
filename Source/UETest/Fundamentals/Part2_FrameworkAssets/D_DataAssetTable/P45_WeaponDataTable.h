// P45 - UDataTable ：CSV / JSON 导入 + 运行时查询
//
// 用法 ：
//   1. 定义继承 FTableRowBase 的 RowStruct
//   2. 编辑器 Content Browser → New → DataTable → 选 RowStruct
//   3. 导入 CSV / JSON 或编辑器内编辑
//   4. C++ FindRow<T> / GetAllRows<T> 查询

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "P45_WeaponDataTable.generated.h"

USTRUCT(BlueprintType)
struct UETEST_API FP45_WeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> Mesh;
};

// Helper ：BP / 测试 用 FindRow
UCLASS()
class UETEST_API UP45_WeaponDataTableHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "P45|DataTable")
	static bool LookupWeapon(UDataTable* Table, FName RowName, FP45_WeaponData& OutData);

	UFUNCTION(BlueprintCallable, Category = "P45|DataTable")
	static int32 CountRows(UDataTable* Table);
};
