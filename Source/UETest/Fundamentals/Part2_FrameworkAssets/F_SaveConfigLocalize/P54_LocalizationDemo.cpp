#include "P54_LocalizationDemo.h"
#include "Internationalization/Text.h"

#define LOCTEXT_NAMESPACE "P54"

FText UP54_LocalizationHelper::GetWelcomeText()
{
	// 用 LOCTEXT 声明 ：编译期写入本地化清单
	return LOCTEXT("Welcome", "欢迎来到 UE5 ！");
}

FText UP54_LocalizationHelper::FormatDamageText(const FString& TargetName, float Amount)
{
	// FText::Format ：模板可被翻译 ；参数运行时填入
	const FText Pattern = LOCTEXT("DamageFormat", "对 {Target} 造成了 {Amount} 点伤害");

	FFormatNamedArguments Args;
	Args.Add(TEXT("Target"), FText::FromString(TargetName));
	Args.Add(TEXT("Amount"), FText::AsNumber(Amount));
	return FText::Format(Pattern, Args);
}

#undef LOCTEXT_NAMESPACE
