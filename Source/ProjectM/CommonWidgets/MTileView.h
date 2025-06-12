// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TileView.h"
#include "Util/MHeap.h"
#include "Inventory/PMInventoryItemList.h"
#include "MTileView.generated.h"

class UMItemDetailData;
class UMItemTileWidget;

#pragma NOTE("이거 드래그앤드랍 사용할거면 타일뷰 쓰면 안돼 위젯 보존이 안돼")
/**
 * 
 */
UCLASS()
class PROJECTM_API UMTileView : public UTileView
{
	GENERATED_BODY()
	
/*
* Overrided Function
*/
public:
	virtual void OnItemDoubleClickedInternal(UObject* Item) override;

/*
* Member Functions
*/
public:
	void InitView(const FPMInventoryItemList& InventoryList, const int32 MaxInventoryCount);
	void InitView(const int32 MaxInventoryCount, EMItemType ItemType = EMItemType::Consumable);
	void AddNewItem(const FPMInventoryEntry& NewItemEntry);
	void RemoveItem(const FMItemHandle& ItemHandle);
	void UpdateEmptySlot(const int32 SlotIndex);

protected:
	UMItemDetailData* GetEmptySlotData(const int32 SlotIndex);

	void AddNewEmptySlot(const int32 SlotIndex);
	void RemoveEmptySlot(const int32 SlotIndex);
	void RequestHeapify();

/*
* Member Variables
*/
protected:
	TMHeap<int32> EmptySlotHeap;
};
