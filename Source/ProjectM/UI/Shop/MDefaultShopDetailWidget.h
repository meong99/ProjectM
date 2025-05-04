// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "Types/MShopTypes.h"
#include "MDefaultShopDetailWidget.generated.h"

class UMDefaultShopSlotWidget;
class UVerticalBox;

UENUM(BlueprintType)
enum class EMShopDetailType : uint8
{
	Shop,
	UserInventory,
};

UCLASS(Abstract)
class PROJECTM_API UMDefaultShopDetailWidget : public UMWidgetBase
{
	GENERATED_BODY()

	/*
	* Overrided Functions
	*/
public:
	UMDefaultShopDetailWidget(const FObjectInitializer& ObjectInitializer);
	virtual void PreAddToLayer() override;

	/*
	* Member Functions
	*/
public:
protected:
	void InitShopDetail();

	/*
	* Member Variables
	*/

protected:
	UPROPERTY(EditAnywhere, Category="ProjectM")
	EMShopDetailType Type = EMShopDetailType::Shop;

	UPROPERTY(BlueprintReadOnly)
	FMShopDefinition ShopDefinition;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="ProjectM")
	TObjectPtr<UVerticalBox> SlotVerticalBox;
};
