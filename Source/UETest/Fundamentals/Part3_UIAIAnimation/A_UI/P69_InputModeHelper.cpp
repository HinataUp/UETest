#include "P69_InputModeHelper.h"
#include "GameFramework/PlayerController.h"
#include "Components/Widget.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Widgets/SWidget.h"

void UP69_InputModeHelper::SetGameOnly(APlayerController* PC)
{
	if (!PC) { return; }
	FInputModeGameOnly Mode;
	PC->SetInputMode(Mode);
	PC->SetShowMouseCursor(false);
}

void UP69_InputModeHelper::SetUIOnly(APlayerController* PC, UWidget* WidgetToFocus)
{
	if (!PC) { return; }
	FInputModeUIOnly Mode;
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	if (WidgetToFocus)
	{
		Mode.SetWidgetToFocus(WidgetToFocus->TakeWidget());
	}
	PC->SetInputMode(Mode);
	PC->SetShowMouseCursor(true);
}

void UP69_InputModeHelper::SetGameAndUI(APlayerController* PC, UWidget* WidgetToFocus)
{
	if (!PC) { return; }
	FInputModeGameAndUI Mode;
	Mode.SetHideCursorDuringCapture(false);
	if (WidgetToFocus)
	{
		Mode.SetWidgetToFocus(WidgetToFocus->TakeWidget());
	}
	PC->SetInputMode(Mode);
	PC->SetShowMouseCursor(true);
}
