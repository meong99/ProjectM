// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MDamageTextWidget.generated.h"

class UOverlay;
class UWidget;
class UMSlideToastText;

UCLASS()
class PROJECTM_API UMDamageTextWidget : public UMWidgetBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMDamageTextWidget(const FObjectInitializer& ObjectInitializer);

	void OnDamaged(const float Damage);

/*
* Member Functions
*/
public:

protected:
	UFUNCTION(BlueprintCallable)
	UMSlideToastText* GetDeactivatedWidget() const;

/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget), Category="ProjectM")
	TObjectPtr<UOverlay> TextOverlay;
};
