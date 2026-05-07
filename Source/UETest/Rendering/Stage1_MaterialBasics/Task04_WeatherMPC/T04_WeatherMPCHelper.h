// T04 - 全局天气 Material Parameter Collection 运行时驱动器
//
// 创建 MPC 资产 / 在材质里引用它都是编辑器操作 ；
// 但从 Gameplay 逻辑动态驱动数值（如天气系统），需要 BP 或 C++ 调用 SetXxxParameterValue 。
// 本类提供一个最小化的 BP 函数库 ，封装常用接口。

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "T04_WeatherMPCHelper.generated.h"

class UMaterialParameterCollection;

UCLASS()
class UETEST_API UT04_WeatherMPCHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 设置湿度（0=干燥 / 1=湿润），所有引用 MPC_Weather 的材质同步生效
	UFUNCTION(BlueprintCallable, Category = "Render|MPC", meta = (WorldContext = "WorldContext"))
	static void SetWetness(const UObject* WorldContext, UMaterialParameterCollection* MPC, float Wetness);

	// 设置雨强 0-1
	UFUNCTION(BlueprintCallable, Category = "Render|MPC", meta = (WorldContext = "WorldContext"))
	static void SetRainIntensity(const UObject* WorldContext, UMaterialParameterCollection* MPC, float Intensity);

	// 设置风向（XYZ 单位向量）
	UFUNCTION(BlueprintCallable, Category = "Render|MPC", meta = (WorldContext = "WorldContext"))
	static void SetWindDirection(const UObject* WorldContext, UMaterialParameterCollection* MPC, FVector Direction);
};
