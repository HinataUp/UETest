#include "P61_HUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "../00_Common/FundP3LogMacros.h"

void UP61_HUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	P3_LOG("P61: HUDWidget NativeConstruct（控件创建后第一次回调）");
	UpdateHealth(100.f, 100.f);
}

void UP61_HUDWidget::UpdateHealth(float Current, float Max)
{
	const float Percent = Max > 0.f ? FMath::Clamp(Current / Max, 0.f, 1.f) : 0.f;

	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
	if (HealthText)
	{
		HealthText->SetText(FText::FromString(
			FString::Printf(TEXT("%.0f / %.0f"), Current, Max)));
	}
}
