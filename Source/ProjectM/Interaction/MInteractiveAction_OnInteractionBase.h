// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MInteractiveAction_Base.h"
#include "MInteractiveAction_OnInteractionBase.generated.h"

UCLASS(Abstract)
class PROJECTM_API UMInteractiveAction_OnInteractionBase : public UMInteractiveAction_Base
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMInteractiveAction_OnInteractionBase(const FObjectInitializer& ObjectInitializer);

/*
* Member Functions
*/
public:
/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Action")
	FText ActionName;
};
