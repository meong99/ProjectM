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
#include "CommonWidgets/MTileView.h"

/*
* UMItemDetailData
*/
void UMItemDetailData::SetNewEntry(const FPMInventoryEntry& NewItemEntry)
{
	if (NewItemEntry.IsValid())
	{
		ItemEntry = NewItemEntry;
	}
}

/*
* UMItemTileWidget
*/
UMItemTileWidget::UMItemTileWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UMItemTileWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
#ifndef WITH_EDITOR
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

	if (ItemData->SlotIndex != INDEX_NONE)
	{
		SetSlotIndex(ItemData->SlotIndex);
	}
	else
	{
		MCHAE_FETAL("Item slot index is not valid! Must be set slot index!");
	}

	OwnerWidget = Cast<UMTileView>(ItemData->GetOuter());
	if (OwnerWidget == nullptr)
	{
		MCHAE_FETAL("Owner of TileWidget is must be MTileView! Check where the tileview is created.");
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
		SwapItemData(NewItemDetailData, GetListItem<UMItemDetailData>());
		UpdateItemData();
		Other->UpdateItemData();
	}

	return true;
}

void UMItemTileWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	MCHAE_LOG("on cancelled");
}

void UMItemTileWidget::UpdateItemData()
{
	UMItemDetailData* ItemDatail = GetListItem<UMItemDetailData>();
	if (ItemDatail == nullptr)
	{
		return;
	}

	const FPMInventoryEntry& NewItemEntry = ItemDatail->ItemEntry;
	if (NewItemEntry.IsValid() == false)
	{
		ResetItemSlot();
	}
	else
	{
		const UPMInventoryItemDefinition* ItemDef = GetDefault<UPMInventoryItemDefinition>(NewItemEntry.Instance->ItemDef);
		ItemImage->SetBrushFromTexture(ItemDef->ItemIcon);
		ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		ItemHandle.ItemUid = NewItemEntry.ItemUid;
	}

	if (OwnerWidget)
	{
		OwnerWidget->UpdateEntryWidget(SlotIndex);
	}
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

void UMItemTileWidget::SwapItemData(UMItemDetailData* Lst, UMItemDetailData* Rst)
{
	if (Lst == nullptr || Rst == nullptr)
	{
		return;
	}

	const FPMInventoryEntry Tmp = Lst->ItemEntry;
	Lst->ItemEntry = Rst->ItemEntry;
	Rst->ItemEntry = Tmp;
}
