// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MItemWithTextWidget.generated.h"

class UImage;
class UTextBlock;
class UTexture2D;

UCLASS()
class PROJECTM_API UMItemWithTextWidget : public UMWidgetBase
{
	GENERATED_BODY()

	/*
	* Overrided Functions
	*/
public:
	UMItemWithTextWidget(const FObjectInitializer& ObjectInitializer);

	/*
	* Member Functions
	*/
public:
	void SetItemTexture(UTexture2D* ItemTexture);
	void SetItemContextText(const FText& ItemContext);
	/*
	* Member Variables
	*/
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UTextBlock> ItemContextText;
};
