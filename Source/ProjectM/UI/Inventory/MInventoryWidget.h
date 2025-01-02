// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MInventoryWidget.generated.h"

class UTileView;
class UPMInventoryManagerComponent;
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
	void Callback_OnInitInventory();

	void Callback_AddNewItem(const FPMInventoryEntry* NewItemEntry);

/*
* Member Variables
*/
protected:
	UPROPERTY()
	UPMInventoryManagerComponent* InventoryComponent;

	UPROPERTY(meta = (BindWidget))
	UTileView* TileView_Items;
};
