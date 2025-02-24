// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TileView.h"
#include "Util/MHeap.h"
#include "Inventory/PMInventoryItemList.h"
#include "MTileView.generated.h"

class UMItemDetailData;

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

/*
* Member Functions
*/
public:
	void InitView(const FPMInventoryItemList& InventoryList, const int32 MaxInventoryCount);
	void AddNewItem(const FPMInventoryEntry& NewItemEntry);
	void RequestHeapify();
	void UpdateEntryWidget(const int32 SlotIndex);

protected:
	UFUNCTION()
	void OnCreate_NewSlotWidget(UObject* Item, UUserWidget* Widget);

	UMItemDetailData* GetEmptySlotData(const int32 SlotIndex);

	void AddNewEmptySlot(const int32 SlotIndex);
	void RemoveEmptySlot(const int32 SlotIndex);
/*
* Member Variables
*/
protected:
	TMHeap<int32> EmptySlotHeap;
};
