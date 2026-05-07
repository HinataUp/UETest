// P61 - UUserWidget 基础 ：BindWidget / NativeConstruct / 数据更新
//
// 关键 ：BindWidget 在蓝图中必须有同名控件 ，否则 Compile 警告
// 用 BindWidgetOptional 允许"该控件可缺失"
//
// 编辑器步骤 ：
//   1) Content Browser 右键 → User Interface → Widget Blueprint → Parent: P61_HUDWidget
//   2) 在 Designer 里加 ：UTextBlock(命名 HealthText) / UProgressBar(命名 HealthBar)
//   3) 创建 ：CreateWidget<UP61_HUDWidget>(PC, WidgetClass)
//   4) 显示 ：HUD->AddToViewport()

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P61_HUDWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class UETEST_API UP61_HUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// BindWidget ：BP 中必须有同名控件 ，否则编译警告
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	// BindWidgetOptional ：BP 中没有也允许（运行时检查 nullptr）
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> PortraitImage;

	// 业务接口 ：外部调用更新血量显示
	UFUNCTION(BlueprintCallable, Category = "P61")
	void UpdateHealth(float Current, float Max);

protected:
	virtual void NativeConstruct() override;
};
