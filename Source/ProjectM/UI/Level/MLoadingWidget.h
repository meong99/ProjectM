// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MLoadingWidget.generated.h"

class UWidgetAnimation;

UCLASS()
class PROJECTM_API UMLoadingWidget : public UMWidgetBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMLoadingWidget(const FObjectInitializer& ObjectInitializer);

	void PlayFadeIn(FWidgetAnimationDynamicEvent&& Callback);
	void PlayFadeOut(FWidgetAnimationDynamicEvent&& Callback);
	void UnbindAnimationBind();
/*
* Member Functions
*/
public:
/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, Transient, meta = (BindWidgetAnim), Category = "ProjectM")
	TObjectPtr<UWidgetAnimation> FadeIn;

	UPROPERTY(EditDefaultsOnly, Transient, meta = (BindWidgetAnim), Category = "ProjectM")
	TObjectPtr<UWidgetAnimation> FadeOut;
};
