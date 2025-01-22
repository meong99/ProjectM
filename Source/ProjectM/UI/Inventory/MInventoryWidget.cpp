#include "MInventoryWidget.h"
#include "Input/PMInputComponent.h"
#include "GameFramework/Pawn.h"
#include "Inventory/PMInventoryManagerComponent.h"
#include "Components/TileView.h"
#include "Items/MItemTileWidget.h"
#include "Inventory/MInventoryTypes.h"

UMInventoryWidget::UMInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UMInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APlayerController* PlayerController = GetOwningPlayer();
	InventoryComponent = PlayerController? PlayerController->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
	if (IsValid(InventoryComponent))
	{
		InventoryComponent->CallOrRegister_OnInitInventory(FOnInitInventory::FDelegate::CreateUObject(this, &ThisClass::Callback_OnInitInventory));
		InventoryComponent->Delegate_OnNewItemAdded.AddUObject(this, &ThisClass::Callback_AddNewItem);
	}
	else
	{
		ensure(false);
		MCHAE_ERROR("Can't get inventorycomponent!");
		return;
	}
}

void UMInventoryWidget::Callback_OnInitInventory(const FPMInventoryList& InventoryList)
{
	if (InventoryComponent)
	{
		const int32 MaxInventoryCount = InventoryComponent->GetMaxInventoryCount();

		for (int32 i = 0; i < MaxInventoryCount; i++)
		{
			UMItemDetailData* ItemDetailData = NewObject<UMItemDetailData>(this);
			if (InventoryList.Entries.IsValidIndex(i))
			{
				ItemDetailData->ItemEntry = InventoryList.Entries[i];
			}
			ItemDetailData->EntryHeight = TileView_Items->GetEntryHeight();
			ItemDetailData->EntryWidth = TileView_Items->GetEntryWidth();
			TileView_Items->AddItem(ItemDetailData);
		}
	}
}

void UMInventoryWidget::Callback_AddNewItem(const FPMInventoryEntry* NewItemEntry)
{
	if (NewItemEntry == nullptr)
	{
		MCHAE_WARNING("NewItemEntry is null");
		return;
	}

	const MPriorityQueueNode<UMItemTileWidget>& EmptySlotHandle = PopEmptySlot();
	if (EmptySlotHandle.IsValid() && EmptySlotHandle.Data)
	{
		EmptySlotHandle.Data->SetNewEntry(*NewItemEntry);
	}
	else
	{
		MCHAE_WARNING("EmptySlot is not valid");
	}
}

void UMInventoryWidget::RegisterEmptySlot(MPriorityQueueNode<UMItemTileWidget>&& NewNode)
{
	EmptySlots.Push_Unique(NewNode);
}

MPriorityQueueNode<UMItemTileWidget> UMInventoryWidget::PopEmptySlot()
{
	MPriorityQueueNode<UMItemTileWidget> Node;
	EmptySlots.Pop(Node);
	return Node;
}
