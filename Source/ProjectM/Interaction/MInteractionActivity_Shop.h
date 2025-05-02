// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MInteractionActivity_Base.h"
#include "MInteractionActivity_Shop.generated.h"

class UInputComponent;

UCLASS()
class PROJECTM_API UMInteractionActivity_Shop : public UMInteractionActivity_Base
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMInteractionActivity_Shop(const FObjectInitializer& ObjectInitializer);

	virtual void InitAction(UMInteractionComponent* InInteractionComponent, AActor* InOwnerActor) override;
	virtual void ActivateAction() override;
	virtual void DeactivateAction() override;

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsActivated = false;
};
