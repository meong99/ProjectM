// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "Util/MPriorityQueue.h"
#include "Inventory/MInventoryTypes.h"
#include "MInventoryWidget.generated.h"

class UTileView;
class UPMInventoryManagerComponent;
class UMItemTileWidget;

struct FPMInventoryEntry;

UCLASS()
class PROJECTM_API UMInventoryWidget : public UMWidgetBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMInventoryWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;

/*
* Member Functions
*/
public:
	void Callback_OnInitInventory(const FPMInventoryList& InventoryList);

	//인벤토리에 존재하지 않던 새로운 아이템이 추가됨
	void Callback_AddNewItem(const FPMInventoryEntry* NewItemEntry);

	void RegisterEmptySlot(MPriorityQueueNode<UMItemTileWidget>&& NewNode);

protected:
	MPriorityQueueNode<UMItemTileWidget> PopEmptySlot();
/*
* Member Variables
*/
protected:
	UPROPERTY()
	UPMInventoryManagerComponent* InventoryComponent;

	UPROPERTY(meta = (BindWidget))
	UTileView* TileView_Items;

	MPriorityQueue<UMItemTileWidget> EmptySlots;
};
