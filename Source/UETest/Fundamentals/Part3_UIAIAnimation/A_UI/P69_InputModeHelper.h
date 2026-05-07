// P69 - Input Mode 管理
//
// SetInputMode_GameOnly       ：游戏控制 ；鼠标隐藏
// SetInputMode_UIOnly         ：纯 UI 操作 ；游戏不接收输入
// SetInputMode_GameAndUI      ：兼顾 ；UI 优先 ，未消费的传给游戏

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P69_InputModeHelper.generated.h"

class APlayerController;

UCLASS()
class UETEST_API UP69_InputModeHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "P69|InputMode")
	static void SetGameOnly(APlayerController* PC);

	UFUNCTION(BlueprintCallable, Category = "P69|InputMode")
	static void SetUIOnly(APlayerController* PC, class UWidget* WidgetToFocus = nullptr);

	UFUNCTION(BlueprintCallable, Category = "P69|InputMode")
	static void SetGameAndUI(APlayerController* PC, class UWidget* WidgetToFocus = nullptr);
};
