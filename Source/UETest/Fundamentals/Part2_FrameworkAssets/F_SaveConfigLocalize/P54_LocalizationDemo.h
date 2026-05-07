// P54 - FText 本地化 + StringTable
//
// 三种本地化原语 ：
//   * NSLOCTEXT(Namespace, Key, Default)  ：声明本地化文本
//   * LOCTEXT(Key, Default)               ：依赖 LOCTEXT_NAMESPACE 宏
//   * FText::Format(Pattern, Args)        ：参数化格式（首选）
//
// StringTable 资产 ：编辑器创建 ；从 ID 反查 FText ；译者友好

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P54_LocalizationDemo.generated.h"

UCLASS()
class UETEST_API UP54_LocalizationHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 返回一个简单本地化文本 ：演示 NSLOCTEXT
	UFUNCTION(BlueprintPure, Category = "P54|Localization")
	static FText GetWelcomeText();

	// 演示 FText::Format ：参数化的"对 X 造成 Y 伤害"模板
	UFUNCTION(BlueprintPure, Category = "P54|Localization")
	static FText FormatDamageText(const FString& TargetName, float Amount);
};
