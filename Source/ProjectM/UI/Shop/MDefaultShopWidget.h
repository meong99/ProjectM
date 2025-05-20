// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MDefaultShopWidget.generated.h"

class UButton;
class UMDefaultShopDetailWidget;

UCLASS(Abstract)
class PROJECTM_API UMDefaultShopWidget : public UMWidgetBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMDefaultShopWidget(const FObjectInitializer& ObjectInitializer);
	virtual void PreAddToLayer() override;
	virtual void NativeConstruct() override;

/*
* Member Functions
*/
public:
protected:
	UFUNCTION()
	void OnClicked_ExitButton();
/*
* Member Variables
*/
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UMDefaultShopDetailWidget> ShopDetail;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UMDefaultShopDetailWidget> UserInventoryDetail;
};
