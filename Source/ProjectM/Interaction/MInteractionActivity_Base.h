// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MInteractionActivity_Base.generated.h"

class UMInteractionComponent;

UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew)
class PROJECTM_API UMInteractionActivity_Base : public UObject
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMInteractionActivity_Base(const FObjectInitializer& ObjectInitializer);

/*
* Member Functions
*/
public:
	virtual void InitAction(UMInteractionComponent* InOwner);
	virtual void ActivateAction() {};
	virtual void DeactivateAction() {};

/*
* Member Variables
*/
protected:
	TWeakObjectPtr<UMInteractionComponent> Owner;
};
