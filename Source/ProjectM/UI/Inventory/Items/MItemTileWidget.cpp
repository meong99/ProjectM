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
#include "Kismet/GameplayStatics.h"

/*
* UMItemDetailData
*/
void UMItemDetailData::SetNewEntry(const FPMInventoryEntry& NewItemEntry)
{
	InitDelegate(NewItemEntry);
	ItemEntry = NewItemEntry;
}

void UMItemDetailData::SwapEntry(UMItemDetailData& Other)
{
	FPMInventoryEntry Tmp = Other.ItemEntry;
	Other.SetNewEntry(ItemEntry);
	SetNewEntry(Tmp);
}

void UMItemDetailData::SwapEntry(UMItemDetailData* Other)
{
	if (Other == nullptr || Other == this)
	{
		MCHAE_WARNING("ItemData is null or change with itself");
		return;
	}

	FPMInventoryEntry Tmp = Other->ItemEntry;
	Other->SetNewEntry(ItemEntry);
	SetNewEntry(Tmp);
}

void UMItemDetailData::OnChangeItemQuantity(const FMItemHandle& ItemHandle, const FMItemResponse& ItemRespons)
{
	UMTileView* MtileView = Cast<UMTileView>(GetOuter());
	if (MtileView)
	{
		MtileView->RegenerateAllEntries();
	}
}

void UMItemDetailData::InitDelegate(const FPMInventoryEntry& NewItemEntry)
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	UPMInventoryManagerComponent* InventoryManager = Controller ? Controller->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
	if (InventoryManager)
	{
		if (ItemEntry.IsValid())
		{
			InventoryManager->RemoveDelegateOnChangeItemQuentity(ItemEntry.GetItemHandle().ItemUid, DelegateHandle);
		}

		if (NewItemEntry.IsValid())
		{
			DelegateHandle = InventoryManager->AddDelegateOnChangeItemQuentity(NewItemEntry.GetItemHandle().ItemUid, FOnChangeItemQuentity::FDelegate::CreateUObject(this, &ThisClass::OnChangeItemQuantity));
		}
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

	if (ItemData->ItemEntry.IsValid())
	{
		ItemData->InitDelegate(ItemData->ItemEntry);
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
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
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

	SwapItemData(Other);
	UpdateItemData();
	Other->UpdateItemData();

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
		ItemHandle.ItemType = NewItemEntry.GetItemType();

// 		if (ItemNum)
// 		{
// 			ItemNum->SetText(NewItemEntry->Instance->GetStatTagStackCount());
// 		}
	}

	K2_UpdateItemData();

	if (OwnerWidget)
	{
		OwnerWidget->UpdateEmptySlot(SlotIndex);
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

void UMItemTileWidget::SwapItemData(UMItemTileWidget* Other)
{
	if (Other)
	{
		UMItemDetailData* OtherData = Other->GetListItem<UMItemDetailData>();
		UMItemDetailData* MyData = GetListItem<UMItemDetailData>();

		if (MyData && OtherData)
		{
			MyData->SwapEntry(OtherData);
		}
	}
}

void UMItemTileWidget::OnItemDoubleClick()
{
	if (ItemHandle.IsValid())
	{
		APlayerController* Controller = GetOwningPlayer();
		UPMInventoryManagerComponent* InventoryManagerComp = Controller ? Controller->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
		if (InventoryManagerComp)
		{
			InventoryManagerComp->Server_UseItem(ItemHandle);
		}
		else
		{
			MCHAE_WARNING("Can't found InventoryComponent!");
		}
	}
}

UMItemDetailData* UMItemTileWidget::GetItemDatailData() const
{
	return GetListItem<UMItemDetailData>();
}
