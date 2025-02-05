#include "MItemTileWidget.h"
#include "Inventory/PMInventoryItemList.h"
#include "Inventory/PMInventoryItemDefinition.h"
#include "Inventory/PMInventoryItemInstance.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "UI/Inventory/MInventoryWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Inventory/PMInventoryManagerComponent.h"
#include "Components/TextBlock.h"

UMItemTileWidget::UMItemTileWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

UE_DISABLE_OPTIMIZATION
void UMItemTileWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
#ifdef WITH_EDITOR
	if (SlotIndex == INDEX_NONE)
	{
		static int32 i = 0;

		SetSlotIndex(i++);
	}
#else
	if (Index)
	{
		Index->SetVisibility(ESlateVisibility::Hidden);
	}
#endif // WITH_EDITOR

	UMItemDetailData* ItemData = Cast<UMItemDetailData>(ListItemObject);
	if (ItemData == nullptr)
	{
		return;
	}

	InventoryWidget = Cast<UMInventoryWidget>(ItemData->GetOuter());
	if (ItemData->ItemEntry.IsValid() == false && InventoryWidget)
	{
		MPriorityQueueNode<UMItemTileWidget> Node;

		Node.Data = this;
		InventoryWidget->RegisterEmptySlot(MoveTemp(Node));
	}

	if (ItemImage)
	{
		ItemImage->SetBrushSize({ ItemData->EntryWidth, ItemData->EntryHeight });
	}

	UpdateItemData();
}

void UMItemTileWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (ItemHandle.IsValid())
	{
		Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	}
}

bool UMItemTileWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (InOperation == nullptr)
	{
		MCHAE_WARNING("Operation is null");
		return false;
	}

	UMItemTileWidget* Other = Cast<UMItemTileWidget>(InOperation->Payload);
	if (Other == nullptr)
	{
		MCHAE_WARNING("payload is not ItemTileWidget!");
		return false;
	}
	else if (Other == this)
	{
		return false;
	}

	UMItemDetailData* NewItemDetailData = Other->GetListItem<UMItemDetailData>();
	if (NewItemDetailData)
	{
		ChangeItemData(NewItemDetailData, GetListItem<UMItemDetailData>());
		UpdateItemData();
		Other->UpdateItemData();
	}

	return true;
}

void UMItemTileWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	MCHAE_LOG("on cancelled");
}

void UMItemTileWidget::SetNewEntry(const FPMInventoryEntry& NewEntry)
{
	UMItemDetailData* NewItemDetailData = GetListItem<UMItemDetailData>();
	if (NewItemDetailData)
	{
		NewItemDetailData->ItemEntry = NewEntry;
		UpdateItemData();
	}
}

void UMItemTileWidget::UpdateItemData()
{
	UMItemDetailData* ItemDatail = GetListItem<UMItemDetailData>();
	if (ItemDatail == nullptr)
	{
		return;
	}

	const FPMInventoryEntry& NewItemEntry = ItemDatail->ItemEntry;
	if (NewItemEntry.IsValid() == false || NewItemEntry.Instance == nullptr)
	{
		ResetItemSlot();
		return;
	}

#pragma TODO("아이템 변경 델리게이트 적용")

	const UPMInventoryItemDefinition* ItemDef = GetDefault<UPMInventoryItemDefinition>(NewItemEntry.Instance->ItemDef);
	ItemImage->SetBrushFromTexture(ItemDef->ItemIcon);
	ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	ItemHandle.ItemUid = NewItemEntry.ItemUid;
}

void UMItemTileWidget::SetSlotIndex(const int32 InIndex)
{
	SlotIndex = InIndex;
	if (Index)
	{
		Index->SetText(FText::FromString(FString::FromInt(SlotIndex)));
	}
}

void UMItemTileWidget::ResetItemSlot()
{
	ItemImage->SetBrushFromTexture(nullptr);
	ItemImage->SetVisibility(ESlateVisibility::Hidden);

	ItemHandle.ItemUid = INDEX_NONE;
}

void UMItemTileWidget::ChangeItemData(UMItemDetailData* Lst, UMItemDetailData* Rst)
{
	if (Lst == nullptr || Rst == nullptr)
	{
		return;
	}

	FPMInventoryEntry Tmp = Lst->ItemEntry;
	Lst->ItemEntry = Rst->ItemEntry;
	Rst->ItemEntry = Tmp;
}
UE_ENABLE_OPTIMIZATION
