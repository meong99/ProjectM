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
	UMInventoryTemplete* InvenOfType = Cast<UMInventoryTemplete>(CreateWidget(this, InventoryTemplete));
	if (InvenOfType)
	{
		Inventories.Add(InvenOfType->GetTileView());
		WidgetSwitcher->AddChild(InvenOfType);
	}
	InitInventorySlots(InventoryList);
}

void UMInventoryWidget::Callback_AddNewItem(const FPMInventoryEntry& NewItemEntry)
{
	if (NewItemEntry.IsValid() == false)
	{
		MCHAE_WARNING("NewItemEntry is not valid");
		return;
	}

	UMTileView* View = GetItemSlotView(NewItemEntry.GetItemType());
	if (View)
	{
		View->AddNewItem(NewItemEntry);
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

void UMInventoryWidget::InitInventorySlots_Impl(const FPMInventoryItemList& InventoryList, UMTileView* ItemSlots)
{
	if (InventoryComponent && ItemSlots)
	{
		const int32 MaxInventoryCount = InventoryComponent->GetMaxInventoryCount();
		ItemSlots->InitView(InventoryList, MaxInventoryCount);
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
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex((int32)EMItemType::Equipment);
	}
}

void UMInventoryWidget::OnClick_ConsumableButton()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex((int32)EMItemType::Consumable);
	}
}

void UMInventoryWidget::OnClick_ExitButton()
{
	RemoveWidgetFromLayer();
}
