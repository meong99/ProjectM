// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MDefaultShopSlotWidget.generated.h"

class UImage;
class UTextBlock;
class UPMInventoryItemDefinition;

UCLASS(Abstract)
class PROJECTM_API UMDefaultShopSlotWidget : public UMWidgetBase
{
	GENERATED_BODY()

	/*
	* Overrided Functions
	*/
public:
	UMDefaultShopSlotWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	/*
	* Member Functions
	*/
public:
	void InitSlot(const int32 InRowId);

protected:
	void OnClickItem();

	/*
	* Member Variables
	*/
protected:
	int32 RowId = INDEX_NONE;

	UPROPERTY()
	TObjectPtr<UPMInventoryItemDefinition> ItemCDO;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UImage> ItemIconBackground;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UImage> PriceImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="ProjectM")
	TObjectPtr<UTextBlock> Price;
};
