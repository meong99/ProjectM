// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "MLevelManager.generated.h"

/**
 *
 */
UCLASS()
class PROJECTM_API UMLevelManager : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
/*
* Overrided Function
*/
public:
	UMLevelManager();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
public:
};
