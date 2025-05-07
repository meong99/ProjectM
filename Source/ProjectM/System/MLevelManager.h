// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "MLevelManager.generated.h"

class UPMUserFacingExperienceDefinition;
class UCommonSessionSubSystem;

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
	UFUNCTION(BlueprintCallable)
	void TravelLevel(UPMUserFacingExperienceDefinition* UFED, const FString& Ip);

protected:
	UFUNCTION()
	void OnLevelLoaded(UWorld* NewWorld);

	UCommonSessionSubSystem* GetSessionSubsystem() const;
	APlayerController* GetPlayerController() const;
/*
* Member Variables
*/
public:
};
