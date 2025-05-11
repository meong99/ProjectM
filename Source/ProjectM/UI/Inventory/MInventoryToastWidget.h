// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "Inventory/MInventoryTypes.h"
#include "MInventoryToastWidget.generated.h"

class UPMInventoryManagerComponent;
class UMWalletComponent;
class UPMExperienceDefinition;

UCLASS()
class PROJECTM_API UMInventoryToastWidget : public UMWidgetBase
{
	GENERATED_BODY()

	/*
	* Overrided Functions
	*/
public:
	UMInventoryToastWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeOnInitialized() override;

/*
* Member Functions
*/
public:
	UFUNCTION(BlueprintCallable)
	void InitThisWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnChangeGold(int64 AdjustPrice, int64 NewGold);
	UFUNCTION()
	void OnChangeGold(int64 AdjustPrice, int64 NewGold);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_NotifyNewItem(const FPMInventoryEntry& ItemEntry);
	void OnNotifyNewItem(const FPMInventoryEntry& ItemEntry, const FMItemResponse& ItemRespons);

protected:
	void OnExperienceLoaded(const UPMExperienceDefinition* LoadedExperienceDefinition);
/*
* Member Variables
*/
protected:
	UPROPERTY(BlueprintReadOnly)
	UPMInventoryManagerComponent* InvenManager;

	UPROPERTY(BlueprintReadOnly)
	UMWalletComponent* WalletComponent;
};
