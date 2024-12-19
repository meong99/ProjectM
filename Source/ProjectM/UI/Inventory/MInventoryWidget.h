// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MInventoryWidget.generated.h"

class UTileView;

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
	UFUNCTION()
	void OnInput_ToggleInventory();

	UFUNCTION()
	void OnPossessedPawn(APawn* OldPawn, APawn* NewPawn);
/*
* Member Variables
*/
protected:
	UPROPERTY(meta = (BindWidget))
	UTileView* TileView_Items;
};
