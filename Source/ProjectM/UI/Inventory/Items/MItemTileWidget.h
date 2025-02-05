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

UCLASS()
class UMItemDetailData : public UObject
{
	GENERATED_BODY()

public:

	bool IsValid() const { return ItemEntry.IsValid(); }

#pragma TODO("이거 포인터가 맞는 듯. Instance가 어떻게 변경되는지 알아야해. 댕글링포인터가 안 생기도록 스마트포인터를 쓰던지 해도 좋을 듯")
	UPROPERTY()
	FPMInventoryEntry ItemEntry;

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

/*
* Member Functions
*/
public:
	void SetNewEntry(const FPMInventoryEntry& NewEntry);
	void UpdateItemData();
	void SetSlotIndex(const int32 InIndex);

protected:
	void ResetItemSlot();

	static void ChangeItemData(UMItemDetailData* Lst, UMItemDetailData* Rst);

/*
* Member Variables
*/
protected:
	UPROPERTY()
	UMInventoryWidget* InventoryWidget;

	int32 SlotIndex = INDEX_NONE;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Index;

	FMItemHandle ItemHandle;
};
