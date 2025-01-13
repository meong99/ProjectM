// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/MDragableWidget.h"
#include "MItemTileWidget.generated.h"

class UTileView;
struct FPMInventoryEntry;
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

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

/*
* Member Functions
*/
public:
	void SetItemData(const FPMInventoryEntry* NewItemEntry);


/*
* Member Variables
*/
protected:
	UPROPERTY()
	UMInventoryWidget* InventoryWidget;
};
