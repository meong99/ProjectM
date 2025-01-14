// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/MDragableWidget.h"
#include "Inventory/MInventoryTypes.h"
#include "MItemTileWidget.generated.h"

class UTileView;
class UImage;
class UMInventoryWidget;

UCLASS()
class UMItemDetailData : public UObject
{
	GENERATED_BODY()

public:
	const FPMInventoryEntry* ItemEntry = nullptr;

	int32 Index = INDEX_NONE;

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
	void SetItemData(const FPMInventoryEntry* NewItemEntry);

protected:
	void ResetItemSlot();


/*
* Member Variables
*/
protected:
	UPROPERTY()
	UMInventoryWidget* InventoryWidget;

	FMItemHandle ItemHandle;
};
