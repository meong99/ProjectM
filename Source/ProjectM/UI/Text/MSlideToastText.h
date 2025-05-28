// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MSlideToastText.generated.h"

class UMaterialInstanceDynamic;
class UTextBlock;
class UWidgetAnimation;

UCLASS()
class PROJECTM_API UMSlideToastText : public UMWidgetBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMSlideToastText(const FObjectInitializer& ObjectInitializer);
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

/*
* Member Functions
*/
public:
	UFUNCTION(BlueprintCallable)
	void SetText(const FText& InText);
	bool IsActivatedText() const { return bIsActivatedText; }

protected:
	void DisableSlideWidget();

/*
* Member Variables
*/
protected:
	UPROPERTY(BlueprintReadWrite, Category="ProjectM")
	bool bIsActivatedText = false;

	UPROPERTY(BlueprintReadWrite, Category="ProjectM")
	TObjectPtr<UMaterialInstanceDynamic> FontMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="ProjectM")
	float AppearancedTime = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ProjectM")
	float MaxAppearanceTime = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget), Category = "ProjectM")
	TObjectPtr<UTextBlock> ToastText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Transient, meta=(BindWidgetAnim), Category = "ProjectM")
	TObjectPtr<UWidgetAnimation> SlideWidgetAnim;
};
