// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MInteractionActivity_Base.h"
#include "GameplayTagContainer.h"
#include "MInteractionActivity_ShowWidget.generated.h"

class UMInteractionComponent;
class UMNameWidgetComponent;

UCLASS()
class PROJECTM_API UMInteractionActivity_ShowWidget : public UMInteractionActivity_Base
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMInteractionActivity_ShowWidget(const FObjectInitializer& ObjectInitializer);

	virtual void InitAction(UMInteractionComponent* InInteractionComponent, AActor* InOwnerActor) override;
	virtual void ActivateAction() override;
	virtual void DeactivateAction() override;

/*
* Member Functions
*/
public:

protected:
	void EnableNameWidget() const;
	void DisableNameWidget() const;

	UMNameWidgetComponent* GetNameWidgetComponent() const;
/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> WidgetTags;
};
