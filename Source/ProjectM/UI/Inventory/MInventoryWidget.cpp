#include "MInventoryWidget.h"
#include "Input/PMInputComponent.h"
#include "GameFramework/Pawn.h"
#include "Inventory/PMInventoryManagerComponent.h"
#include "Items/MItemTileWidget.h"
#include "Inventory/PMInventoryItemList.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "CommonWidgets/MTileWidget.h"

UMInventoryWidget::UMInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UMInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BindDelegates();
}

FReply UMInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UMInventoryWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UMInventoryWidget::Callback_OnInitInventory(const FPMInventoryItemList& InventoryList)
{
	InitInventorySlots(InventoryList);
}

void UMInventoryWidget::Callback_AddNewItem(const FPMInventoryEntry* NewItemEntry)
{
	if (NewItemEntry == nullptr)
	{
		MCHAE_WARNING("NewItemEntry is null");
		return;
	}

	const MPriorityQueueNode<UMItemTileWidget>& EmptySlotHandle = PopEmptySlot(NewItemEntry->GetItemType());
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
	if (NewNode.ItemType == (int)EMItemType::Equipment)
	{
		EmptySlots.Push_Unique(NewNode);
	}
	else if (NewNode.ItemType == (int)EMItemType::Consumable)
	{
		ConsumableEmptySlots.Push_Unique(NewNode);
	}
}

void UMInventoryWidget::BindDelegates()
{
	APlayerController* PlayerController = GetOwningPlayer();
	InventoryComponent = PlayerController ? PlayerController->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
	if (IsValid(InventoryComponent))
	{
		InventoryComponent->CallOrRegister_OnInitInventory(FOnInitInventory::FDelegate::CreateUObject(this, &ThisClass::Callback_OnInitInventory));
		InventoryComponent->Delegate_OnNewItemAdded.AddUObject(this, &ThisClass::Callback_AddNewItem);
	}
	else
	{
		ensure(false);
		MCHAE_ERROR("Can't get inventorycomponent!");
	}

	if (EquipmentButton)
	{
		EquipmentButton->OnClicked.AddDynamic(this, &ThisClass::OnClick_EquipmentButton);
	}
	if (ConsumableButton)
	{
		ConsumableButton->OnClicked.AddDynamic(this, &ThisClass::OnClick_ConsumableButton);
	}
	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &ThisClass::OnClick_ExitButton);
	}
}

void UMInventoryWidget::InitInventorySlots(const FPMInventoryItemList& InventoryList)
{
	InitInventorySlots_Impl(InventoryList, GetItemSlotView(InventoryList.OwnedItemType));
}

void UMInventoryWidget::InitInventorySlots_Impl(const FPMInventoryItemList& InventoryList, UTileView* ItemSlots)
{
	if (InventoryComponent && ItemSlots)
	{
		const int32 MaxInventoryCount = InventoryComponent->GetMaxInventoryCount();

		auto EntryIter = InventoryList.Entries.CreateConstIterator();
		for (int32 i = 0; i < MaxInventoryCount; i++)
		{
			UMItemDetailData* ItemDetailData = NewObject<UMItemDetailData>(this);
			if (EntryIter)
			{
				ItemDetailData->ItemEntry = *EntryIter;
				++EntryIter;
			}
			ItemDetailData->SlotIndex = i;
			ItemDetailData->SlotType = InventoryList.OwnedItemType;
			ItemDetailData->EntryHeight = ItemSlots->GetEntryHeight();
			ItemDetailData->EntryWidth = ItemSlots->GetEntryWidth();
			ItemSlots->AddItem(ItemDetailData);
		}
	}
}

UTileView* UMInventoryWidget::GetItemSlotView(const EMItemType ItemType)
{
	switch (ItemType)
	{
		case EMItemType::Equipment :
		{
			return TileView_EquipmentItems;
		}
		case EMItemType::Consumable :
		{
			return TileView_ConsumableItems;
		}
		default:
			return nullptr;
	}
}

void UMInventoryWidget::OnClick_EquipmentButton()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
	}
}

void UMInventoryWidget::OnClick_ConsumableButton()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(1);
	}
}

void UMInventoryWidget::OnClick_ExitButton()
{
	RemoveWidgetFromLayer();
}

MPriorityQueueNode<UMItemTileWidget> UMInventoryWidget::PopEmptySlot(EMItemType ItemType)
{
	MPriorityQueueNode<UMItemTileWidget> Node;

	if (ItemType == EMItemType::Equipment)
	{
		EmptySlots.Pop(Node);
	}
	else if (ItemType == EMItemType::Consumable)
	{
		ConsumableEmptySlots.Pop(Node);
	}

	return Node;
}
