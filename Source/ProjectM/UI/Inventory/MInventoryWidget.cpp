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
		InventoryComponent->CallOrRegister_FinishInventoryInit(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::Callback_OnInitInventory));
		InventoryComponent->Delegate_OnNewItemAdded.AddUObject(this, &ThisClass::Callback_AddNewItem);
	}
	else
	{
		ensure(false);
		MCHAE_ERROR("Can't get inventorycomponent!");
		return;
	}
}

void UMInventoryWidget::Callback_OnInitInventory()
{
	if (InventoryComponent)
	{
		const int32 MaxInventoryCount = InventoryComponent->GetMaxInventoryCount();

		for (int32 i = 0; i < MaxInventoryCount; i++)
		{
			UMItemDetailData* ItemDetailData = NewObject<UMItemDetailData>(this);

			TileView_Items->AddItem(ItemDetailData);
		}
	}
}

void UMInventoryWidget::Callback_AddNewItem(const FPMInventoryEntry* NewItemEntry)
{

}
