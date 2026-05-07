// P98 - Packaging / Cooking / Validation
//
// 与 Part 2 的 P58 重叠 ：本 Helper 提供更"业务向"的 BP 接口
// 实战 ：在游戏中通过此接口判断"是否是开发版"，决定是否显示调试 UI

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P98_BuildInfoHelper.generated.h"

UENUM(BlueprintType)
enum class EP98_BuildConfig : uint8
{
	Debug,
	DebugGame,
	Development,
	Test,
	Shipping,
	Unknown
};

UCLASS()
class UETEST_API UP98_BuildInfoHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 当前构建配置
	UFUNCTION(BlueprintPure, Category = "P98|Build")
	static EP98_BuildConfig GetBuildConfiguration();

	// 是否带 Editor（DevelopmentEditor / DebugGameEditor）
	UFUNCTION(BlueprintPure, Category = "P98|Build")
	static bool IsEditorBuild();

	// 是否 Shipping（性能 / 安全敏感 ；调试代码不应跑）
	UFUNCTION(BlueprintPure, Category = "P98|Build")
	static bool IsShippingBuild();

	// 是否带调试输出（非 Shipping）
	UFUNCTION(BlueprintPure, Category = "P98|Build")
	static bool HasDebugOutput();
};
