#include "P62_MenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../00_Common/FundP3LogMacros.h"

void UP62_MenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		// AddDynamic 必须绑 UFUNCTION ；事件绑定标准模式
		StartButton->OnClicked.AddDynamic(this, &UP62_MenuWidget::OnStartClicked);
		StartButton->OnHovered.AddDynamic(this, &UP62_MenuWidget::OnStartHovered);
	}
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UP62_MenuWidget::OnQuitClicked);
	}
	if (StatusText)
	{
		StatusText->SetText(FText::FromString(TEXT("准备就绪")));
	}
}

void UP62_MenuWidget::NativeDestruct()
{
	// 解绑（虽然 Widget 销毁时按钮也销毁 ；但保持对称习惯）
	if (StartButton)
	{
		StartButton->OnClicked.RemoveAll(this);
		StartButton->OnHovered.RemoveAll(this);
	}
	if (QuitButton)
	{
		QuitButton->OnClicked.RemoveAll(this);
	}
	Super::NativeDestruct();
}

void UP62_MenuWidget::OnStartClicked()
{
	P3_LOG("P62: StartButton 被点击");
	if (StatusText)
	{
		StatusText->SetText(FText::FromString(TEXT("开始游戏...")));
	}
	OnStartGameRequested.Broadcast();
}

void UP62_MenuWidget::OnStartHovered()
{
	if (StatusText)
	{
		StatusText->SetText(FText::FromString(TEXT("点击开始游戏")));
	}
}

void UP62_MenuWidget::OnQuitClicked()
{
	P3_LOG("P62: QuitButton 被点击 → 退出");
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
