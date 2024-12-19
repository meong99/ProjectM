// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "MWidgetBase.generated.h"

UCLASS()
class PROJECTM_API UMWidgetBase : public UUserWidget
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMWidgetBase(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;

/*
* Member Functions
*/
public:

	const FGameplayTag& GetWidgetTag() const { return WidgetTag; }

	void SetWidgetTag(const FGameplayTag& InWidgetTag) { WidgetTag = InWidgetTag; }
/*
* Member Variables
*/
protected:
	UPROPERTY(VisibleInstanceOnly)
	FGameplayTag WidgetTag;
};
