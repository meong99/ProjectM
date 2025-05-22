#include "MInventoryWidget.h"
#include "Input/PMInputComponent.h"
#include "GameFramework/Pawn.h"
#include "Inventory/PMInventoryManagerComponent.h"
#include "Items/MItemTileWidget.h"
#include "Inventory/PMInventoryItemList.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "CommonWidgets/MTileView.h"
#include "MInventoryTemplete.h"
#include "Inventory/PMInventoryItemInstance.h"

UMInventoryWidget::UMInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UMInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BindDelegates();
}

void UMInventoryWidget::Callback_OnInitInventory(const FPMInventoryItemList& InventoryList)
{
	UMInventoryTemplete* InvenOfType = Cast<UMInventoryTemplete>(CreateWidget(this, InventoryTemplete));
	if (InvenOfType)
	{
		Inventories.Add(InvenOfType->GetTileView());
		InventoryTileSwitcher->AddChild(InvenOfType);
	}
	InitInventorySlots(InventoryList);
}

void UMInventoryWidget::Callback_AddNewItem(const FMItemResponse& ItemRespons)
{
	if (ItemRespons.ResponsType == EMItemResponseType::TotallyNewItem)
	{
		APlayerController* PlayerController = GetOwningPlayer();
		InventoryComponent = PlayerController ? PlayerController->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
		if (IsValid(InventoryComponent))
		{
			FPMInventoryEntry* Entry = InventoryComponent->FindEntry(ItemRespons.ItemRequest.ItemHandle);
			if (Entry)
			{
				UMTileView* View = GetItemSlotView(Entry->GetItemType());
				if (View)
				{
					View->AddNewItem(*Entry);
				}
			}
		}
	}
}

void UMInventoryWidget::Callback_RemoveItem(const FMItemResponse& ItemRespons)
{
	if (ItemRespons.ResponsType == EMItemResponseType::Removed && ItemRespons.ItemRequest.ItemInstance)
	{
		UMTileView* View = GetItemSlotView(ItemRespons.ItemRequest.ItemInstance->GetItemType());
		if (View)
		{
			View->RemoveItem(ItemRespons.ItemRequest.ItemHandle);
		}
	}
}

void UMInventoryWidget::BindDelegates()
{
	APlayerController* PlayerController = GetOwningPlayer();
	InventoryComponent = PlayerController ? PlayerController->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
	if (IsValid(InventoryComponent))
	{
		InventoryComponent->CallOrRegister_OnInitInventory(FOnInitInventory::FDelegate::CreateUObject(this, &ThisClass::Callback_OnInitInventory));
		InventoryComponent->Delegate_OnItemIncreased.AddUObject(this, &ThisClass::Callback_AddNewItem);
		InventoryComponent->Delegate_OnRemoveItem.AddUObject(this, &ThisClass::Callback_RemoveItem);
	}
	else
	{
		ensure(false);
		MCHAE_ERROR("Can't get inventorycomponent!");
	}

	if (EquipmentButton_Deactivated)
	{
		EquipmentButton_Deactivated->OnClicked.AddDynamic(this, &ThisClass::OnClick_EquipmentButton);
	}
	if (ConsumableButton_Deactivated)
	{
		ConsumableButton_Deactivated->OnClicked.AddDynamic(this, &ThisClass::OnClick_ConsumableButton);
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

void UMInventoryWidget::InitInventorySlots_Impl(const FPMInventoryItemList& InventoryList, UMTileView* ItemSlotView)
{
	if (InventoryComponent && ItemSlotView)
	{
		const int32 MaxInventoryCount = InventoryComponent->GetMaxInventoryCount();
		ItemSlotView->InitView(InventoryList, MaxInventoryCount);
	}
}

UMTileView* UMInventoryWidget::GetItemSlotView(const EMItemType ItemType)
{
	if (Inventories.IsValidIndex((int32)ItemType))
	{
		return Inventories[(int32)ItemType];
	}
	
	return nullptr;
}

void UMInventoryWidget::OnClick_EquipmentButton()
{
	InventoryTileSwitcher->SetActiveWidgetIndex((int32)EMItemType::Equipment);
	EquipButtonSwitcher->SetActiveWidgetIndex((int32)EMButtonState::Active);
	ConsumableButtonSwitcher->SetActiveWidgetIndex((int32)EMButtonState::Deactive);
}

void UMInventoryWidget::OnClick_ConsumableButton()
{
	InventoryTileSwitcher->SetActiveWidgetIndex((int32)EMItemType::Consumable);
	EquipButtonSwitcher->SetActiveWidgetIndex((int32)EMButtonState::Deactive);
	ConsumableButtonSwitcher->SetActiveWidgetIndex((int32)EMButtonState::Active);
}

void UMInventoryWidget::OnClick_ExitButton()
{
	RemoveWidgetFromLayer();
}
