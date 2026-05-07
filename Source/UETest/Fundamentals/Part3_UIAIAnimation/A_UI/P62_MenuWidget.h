// P62 - UMG 事件绑定 + 数据驱动模式
//
// 推荐"手动更新"而非 Property Binding ：
//   * Property Binding 每帧反复调 ，性能差
//   * 手动 ：Subsystem / Controller 在数据变化时调 Widget->UpdateXxx

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P62_MenuWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FP62_OnStartGame);

UCLASS()
class UETEST_API UP62_MenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StatusText;

	// 暴露给 BP / 外部 C++ 监听"开始游戏"
	UPROPERTY(BlueprintAssignable, Category = "P62")
	FP62_OnStartGame OnStartGameRequested;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION() void OnStartClicked();
	UFUNCTION() void OnStartHovered();
	UFUNCTION() void OnQuitClicked();
};
