// P65 - Slate 基础 ：SCompoundWidget 自定义控件
//
// Slate 是 UMG 的底层 ；纯 C++ DSL 描述 UI
// SLATE_BEGIN_ARGS / SLATE_ARGUMENT / SLATE_EVENT 模板宏定义构造参数
// 用法 ：SAssignNew(MyPanel, SP65_Panel).Title(...).OnButtonClicked(...);

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class UETEST_API SP65_Panel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SP65_Panel) : _Title(NSLOCTEXT("P65", "Default", "默认标题")) {}
		SLATE_ARGUMENT(FText, Title)
		SLATE_EVENT(FOnClicked, OnButtonClicked)
	SLATE_END_ARGS()

	// 构造 ：在 InArgs 中拿外部传入的参数 ；用 SNew 组合子控件
	void Construct(const FArguments& InArgs);
};
