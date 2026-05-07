#include "P63_InventoryListView.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "../00_Common/FundP3LogMacros.h"

void UP63_InventorySlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	// ListView 复用控件时调用此函数 ；把数据填到控件
	UP63_InventoryItemData* Data = Cast<UP63_InventoryItemData>(ListItemObject);
	if (!Data) { return; }

	if (ItemNameText) { ItemNameText->SetText(Data->DisplayName); }
	if (ItemCountText)
	{
		ItemCountText->SetText(FText::FromString(FString::Printf(TEXT("x%d"), Data->Count)));
	}
}

void UP63_InventoryListWidget::SetItems(const TArray<UP63_InventoryItemData*>& Items)
{
	if (!InventoryListView) { return; }

	// SetListItems 触发 ListView 内部"虚拟化"机制 ：
	// 自动按可见区域创建 / 复用 EntryWidget ；远超条目数时控件池保持小
	TArray<UObject*> AsObjs;
	AsObjs.Reserve(Items.Num());
	for (UP63_InventoryItemData* It : Items) { AsObjs.Add(It); }
	InventoryListView->SetListItems(AsObjs);
	P3_LOG("P63: ListView SetListItems 数量 = %d", Items.Num());
}
