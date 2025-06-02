// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MContextableItemWidget.generated.h"

class UImage;

UCLASS()
class PROJECTM_API UMContextableItemWidget : public UMWidgetBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMContextableItemWidget(const FObjectInitializer& ObjectInitializer);
protected:

/*
* Member Functions
*/
public:
	void SetDesiredSizeOverride(FVector2D DesiredSize);
	void SetBrushFromTexture(UTexture2D* Texture, bool bMatchSize = false);
	void SetItemRowId(const int32 InRowId) { ItemRowId = InRowId; OnSetRowId(); }

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetRowId();
/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UImage* ItemImage;

	UPROPERTY(BlueprintReadOnly)
	int32 ItemRowId = INDEX_NONE;
};
