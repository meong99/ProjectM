// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PMActivatableWidget.h"
#include "MInventoryWidget.generated.h"

class UTileView;

UCLASS()
class PROJECTM_API UMInventoryWidget : public UPMActivatableWidget
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
	UFUNCTION()
	void OnInput_ToggleInventory();


/*
* Member Variables
*/
protected:
	UPROPERTY(meta = (BindWidget))
	UTileView* TileView_Items;
};
