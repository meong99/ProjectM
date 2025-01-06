#include "MItemTileWidget.h"
#include "Inventory/MInventoryTypes.h"
#include "Inventory/PMInventoryItemDefinition.h"
#include "Inventory/PMInventoryItemInstance.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "UI/Inventory/MInventoryWidget.h"

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
	ItemIcon->SetBrushFromTexture(ItemDef->ItemIcon);
}
