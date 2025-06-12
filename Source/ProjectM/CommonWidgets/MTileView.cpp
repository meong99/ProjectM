// Fill out your copyright notice in the Description page of Project Settings.

#include "CommonWidgets/MTileView.h"
#include "UI/Inventory/Items/MItemTileWidget.h"

void UMTileView::OnItemDoubleClickedInternal(UObject* Item)
{
	Super::OnItemDoubleClickedInternal(Item);

	UMItemTileWidget* EntryWidget = Cast<UMItemTileWidget>(GetEntryWidgetFromItem(Item));
	if (EntryWidget)
	{
		EntryWidget->OnItemDoubleClick();
	}
}

void UMTileView::InitView(const FPMInventoryItemList& InventoryList, const int32 MaxInventoryCount)
{
	auto EntryIter = InventoryList.Entries.CreateConstIterator();
	for (int32 i = 0; i < MaxInventoryCount; i++)
	{
		UMItemDetailData* ItemDetailData = NewObject<UMItemDetailData>(this);
		if (EntryIter)
		{
			ItemDetailData->ItemEntry = *EntryIter;
			++EntryIter;
		}
		else
		{
			AddNewEmptySlot(i);
		}
		ItemDetailData->SlotIndex = i;
		ItemDetailData->SlotType = InventoryList.OwnedItemType;
		ItemDetailData->EntryHeight = GetEntryHeight();
		ItemDetailData->EntryWidth = GetEntryWidth();
		AddItem(ItemDetailData);
	}
}

void UMTileView::InitView(const int32 MaxInventoryCount, EMItemType ItemType /*= EMItemType::Consumable*/)
{
	for (int32 i = 0; i < MaxInventoryCount; i++)
	{
		UMItemDetailData* ItemDetailData = NewObject<UMItemDetailData>(this);
		AddNewEmptySlot(i);
		ItemDetailData->SlotIndex = i;
		ItemDetailData->SlotType = ItemType;
		ItemDetailData->EntryHeight = GetEntryHeight();
		ItemDetailData->EntryWidth = GetEntryWidth();
		AddItem(ItemDetailData);
	}
}

void UMTileView::AddNewItem(const FPMInventoryEntry& NewItemEntry)
{
	if (EmptySlotHeap.IsEmpty())
	{
		MCHAE_WARNING("Inventory is full");
		return;
	}

	int32 EmptySlotIndex = EmptySlotHeap.Pop();
	UMItemDetailData* EmptySlotData = GetEmptySlotData(EmptySlotIndex);

	if (EmptySlotData)
	{
		EmptySlotData->SetNewEntry(NewItemEntry);
		RegenerateAllEntries();
	}
	else
	{
		MCHAE_WARNING("EmptySlot is not valid");
	}
}

void UMTileView::RemoveItem(const FMItemHandle& ItemHandle)
{
	const TArray<UObject*>& List = GetListItems();
	for (UObject* Item : List)
	{
		UMItemDetailData* Data = Cast<UMItemDetailData>(Item);
		if (Data && Data->ItemEntry.ItemUid == ItemHandle.ItemUid)
		{
			Data->SetNewEntry({});
			UpdateEmptySlot(Data->SlotIndex);
			RegenerateAllEntries();
			break;
		}
	}
}

void UMTileView::AddNewEmptySlot(const int32 SlotIndex)
{
	EmptySlotHeap.Push(SlotIndex);
}

void UMTileView::RemoveEmptySlot(const int32 SlotIndex)
{
	EmptySlotHeap.RemoveElement(SlotIndex);
}

void UMTileView::RequestHeapify()
{
	EmptySlotHeap.Heapify();
}

void UMTileView::UpdateEmptySlot(const int32 SlotIndex)
{
	UMItemDetailData* SlotData = GetEmptySlotData(SlotIndex);
	if (SlotData == nullptr)
	{
		return;
	}

	if (SlotData->ItemEntry.IsValid())
	{
		RemoveEmptySlot(SlotIndex);
	}
	else
	{
		AddNewEmptySlot(SlotIndex);
	}
}

UMItemDetailData* UMTileView::GetEmptySlotData(const int32 SlotIndex)
{
	return Cast<UMItemDetailData>(GetItemAt(SlotIndex));
}
