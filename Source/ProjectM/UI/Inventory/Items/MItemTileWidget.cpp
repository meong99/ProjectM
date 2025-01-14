#include "MItemTileWidget.h"
#include "Inventory/MInventoryTypes.h"
#include "Inventory/PMInventoryItemDefinition.h"
#include "Inventory/PMInventoryItemInstance.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "UI/Inventory/MInventoryWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "Inventory/PMInventoryManagerComponent.h"

UMItemTileWidget::UMItemTileWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UMItemTileWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UMItemDetailData* ItemData = Cast<UMItemDetailData>(ListItemObject);
	if (ItemData == nullptr)
	{
		return;
	}

	InventoryWidget = Cast<UMInventoryWidget>(ItemData->GetOuter());
	if (ItemData->ItemEntry == nullptr && InventoryWidget)
	{
		MPriorityQueueNode<UMItemTileWidget> Node;

		Node.Keyid = ItemData->Index;
		Node.Data = this;
		InventoryWidget->RegisterEmptySlot(MoveTemp(Node));
	}
	else if (ItemData->ItemEntry)
	{
		SetItemData(ItemData->ItemEntry);
	}

	if (ItemImage)
	{
		ItemImage->SetBrushSize({ ItemData->EntryWidth, ItemData->EntryHeight });
	}
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

	APlayerController* Controller = GetOwningPlayer();
	UPMInventoryManagerComponent* InvenManager = Controller ? Controller->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
	if (Controller == nullptr)
	{
		MCHAE_WARNING("Can't get InventoryManager");
		return false;
	}

	const FPMInventoryEntry* NewItemEntry = InvenManager->FindEntry(Other->ItemHandle);
	if (NewItemEntry == nullptr)
	{
		MCHAE_WARNING("Can't Found Entry");
		return false;
	}

	SetItemData(NewItemEntry);
	Other->ResetItemSlot();

	return true;
}

void UMItemTileWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	MCHAE_LOG("on cancelled");
}

void UMItemTileWidget::SetItemData(const FPMInventoryEntry* NewItemEntry)
{
	if (NewItemEntry == nullptr || NewItemEntry->Instance == nullptr)
	{
		MCHAE_WARNING("You trying to set invalid item into widget!");
		return;
	}

#pragma TODO("아이템 변경 델리게이트 적용")

	const UPMInventoryItemDefinition* ItemDef = GetDefault<UPMInventoryItemDefinition>(NewItemEntry->Instance->ItemDef);
	ItemImage->SetBrushFromTexture(ItemDef->ItemIcon);

	ItemHandle.ItemUid = NewItemEntry->ItemUid;
}

void UMItemTileWidget::ResetItemSlot()
{
	ItemImage->SetBrushFromTexture(nullptr);

	ItemHandle.ItemUid = INDEX_NONE;
}
