// P63 - UListView ：数据驱动列表
//
// 关键设计 ：
//   * 数据对象 ：UObject 子类 ，实现 IUserObjectListEntry（其实只需是 UObject）
//   * 条目控件 ：UUserWidget 子类 ，实现 IUserObjectListEntry
//   * ListView ：SetListItems(DataArray) ，自动复用控件（虚拟列表）

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "P63_InventoryListView.generated.h"

// 数据载体 ：纯数据 UObject
UCLASS(BlueprintType)
class UETEST_API UP63_InventoryItemData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite) FName ItemId;
	UPROPERTY(BlueprintReadWrite) FText DisplayName;
	UPROPERTY(BlueprintReadWrite) int32 Count = 0;
};

// 单个条目控件 ：实现 IUserObjectListEntry 接收 ListView 通知
UCLASS()
class UETEST_API UP63_InventorySlotWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemCountText;

protected:
	// IUserObjectListEntry ：ListView 通知本控件去显示 ListItemObject
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};

// 容器 ：包含一个 UListView
UCLASS()
class UETEST_API UP63_InventoryListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UListView> InventoryListView;

	// 业务接口 ：从外部传入物品数据
	UFUNCTION(BlueprintCallable, Category = "P63")
	void SetItems(const TArray<UP63_InventoryItemData*>& Items);
};
