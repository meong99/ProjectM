// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/MDragableWidget.h"

#pragma TODO("포인터로 변경하면 아래 헤더 바꿔")
#include "Inventory/PMInventoryItemList.h"
#include "MItemTileWidget.generated.h"

class UTileView;
class UImage;
class UMInventoryWidget;
class UTextBlock;
class UMTileView;

UCLASS()
class UMItemDetailData : public UObject
{
	GENERATED_BODY()

public:
#pragma TODO("아이템 변경 델리게이트 적용")
	bool IsValid() const { return ItemEntry.IsValid(); }
	void SetNewEntry(const FPMInventoryEntry& NewItemEntry);

	FPMInventoryEntry ItemEntry;

	int32 SlotIndex = INDEX_NONE;

	EMItemType SlotType = EMItemType::None;

	float EntryHeight;

	float EntryWidth;
};

UCLASS()
class PROJECTM_API UMItemTileWidget : public UMDragableWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMItemTileWidget(const FObjectInitializer& ObjectInitializer);

	virtual void	NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void	NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool	NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void	NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply	NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	/*
* Member Functions
*/
public:
	void UpdateItemData();
	void SetSlotIndex(const int32 InIndex);

protected:
	void ResetItemSlot();

	static void SwapItemData(UMItemDetailData* Lst, UMItemDetailData* Rst);

/*
* Member Variables
*/
protected:
	UPROPERTY()
	UMTileView* OwnerWidget;

	int32 SlotIndex = INDEX_NONE;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Index;

	FMItemHandle ItemHandle;
};
