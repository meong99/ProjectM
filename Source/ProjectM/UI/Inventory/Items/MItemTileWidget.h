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

UCLASS(BlueprintType)
class UMItemDetailData : public UObject
{
	GENERATED_BODY()

public:
#pragma TODO("아이템 변경 델리게이트 적용")
	bool IsValid() const { return ItemEntry.IsValid(); }
	void SetNewEntry(const FPMInventoryEntry& NewItemEntry);
	void SwapEntry(UMItemDetailData& Other);
	void SwapEntry(UMItemDetailData* Other);
	void OnChangeItemQuantity(const FMItemHandle& ItemHandle, const FMItemResponse& ItemRespons);
	void InitDelegate(const FPMInventoryEntry& NewItemEntry);

	UPROPERTY(BlueprintReadWrite)
	FPMInventoryEntry ItemEntry;

	UPROPERTY(BlueprintReadWrite)
	int32 SlotIndex = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite)
	EMItemType SlotType = EMItemType::None;

	UPROPERTY(BlueprintReadWrite)
	float EntryHeight;

	UPROPERTY(BlueprintReadWrite)
	float EntryWidth;

	FDelegateHandle DelegateHandle;
};

UCLASS(BlueprintType)
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

/*
* Member Functions
*/
public:
	void SwapItemData(UMItemTileWidget* Other);
	void OnItemDoubleClick();

	UFUNCTION(BlueprintCallable)
	UMItemDetailData* GetItemDatailData() const;
	UFUNCTION(BlueprintImplementableEvent)
	void K2_UpdateItemData();

protected:
	void UpdateItemData();
	void SetSlotIndex(const int32 InIndex);
	void ResetItemSlot();

/*
* Member Variables
*/
protected:
	UPROPERTY(BlueprintReadOnly)
	UMTileView* OwnerWidget;

	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex = INDEX_NONE;
	
// 	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
// 	UTextBlock* ItemNum;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* Index;

	UPROPERTY(BlueprintReadOnly)
	FMItemHandle ItemHandle;
};
