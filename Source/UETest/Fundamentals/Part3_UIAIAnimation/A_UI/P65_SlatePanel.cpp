#include "P65_SlatePanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "P65"

void SP65_Panel::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(300.f).HeightOverride(120.f)
		[
			SNew(SVerticalBox)

			// 第一行 ：标题
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.f)
			[
				SNew(STextBlock).Text(InArgs._Title)
			]

			// 第二行 ：按钮
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(8.f)
			[
				SNew(SButton)
				.OnClicked(InArgs._OnButtonClicked)
				[
					SNew(STextBlock).Text(LOCTEXT("Click", "点击我"))
				]
			]
		]
	];
}

#undef LOCTEXT_NAMESPACE
