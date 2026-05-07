// P95 - Dynamic Material Instance + Material Parameter Collection
//
// MID ：每实例自定义参数（受击闪白 / 角色染色）
// MPC ：跨材质共享全局参数（天气湿度 / 时间 ：所有材质共用）
//
// 与 Rendering 模块的 T11 / T04 概念一致 ；本 Helper 简洁版便于 Part 4 学习

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P95_MaterialMPCHelper.generated.h"

class UMeshComponent;
class UMaterialInstanceDynamic;
class UMaterialParameterCollection;

UCLASS()
class UETEST_API UP95_MaterialMPCHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// MID 一站式 ：CreateDynamic + 立即 SetScalar
	UFUNCTION(BlueprintCallable, Category = "P95|MID")
	static UMaterialInstanceDynamic* CreateMIDAndSet(
		UMeshComponent* Mesh, int32 SlotIndex,
		FName ScalarName, float ScalarValue);

	// MPC ：拿到 World 实例 ， SetScalar
	UFUNCTION(BlueprintCallable, Category = "P95|MPC",
		meta = (WorldContext = "WorldContext"))
	static void SetMPCScalar(
		const UObject* WorldContext, UMaterialParameterCollection* MPC,
		FName ScalarName, float Value);

	UFUNCTION(BlueprintCallable, Category = "P95|MPC",
		meta = (WorldContext = "WorldContext"))
	static void SetMPCVector(
		const UObject* WorldContext, UMaterialParameterCollection* MPC,
		FName VectorName, FLinearColor Value);
};
