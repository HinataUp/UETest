// P58 - 预处理 / 平台 / 构建配置 宏
//
// WITH_EDITOR        ：编辑器构建（Editor / DevelopmentEditor）才为 1
// !UE_BUILD_SHIPPING ：Shipping 包之外都为 1（含 Test）
// PLATFORM_WINDOWS / PLATFORM_MAC / PLATFORM_LINUX
// TEXT(...)          ：UE 字符串字面量包装（生成 wchar_t* 或 char* 取决于平台）

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P58_PreprocessorDemo.generated.h"

UCLASS()
class UETEST_API UP58_PreprocessorHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 仅编辑器逻辑 ：Shipping 中此函数体为空（仍存在导出 ，避免链接错）
	UFUNCTION(BlueprintCallable, Category = "P58|Build")
	static void EditorOnlyDebugDraw();

	// 返回当前构建是否为 Shipping
	UFUNCTION(BlueprintPure, Category = "P58|Build")
	static bool IsShippingBuild();

	// 返回平台名（Windows / Mac / Linux ...）
	UFUNCTION(BlueprintPure, Category = "P58|Build")
	static FString GetPlatformName();
};
