// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "MControllerComponentBase.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class PROJECTM_API UMControllerComponentBase : public UControllerComponent
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMControllerComponentBase(const FObjectInitializer& ObjectInitializer);

/*
* Member Functions
*/
public:
	virtual void OnServerRestartPlayer() {}
	virtual void OnClientRestart(APawn* NewPawn) {}

protected:
/*
* Member Variables
*/
protected:
	
};
