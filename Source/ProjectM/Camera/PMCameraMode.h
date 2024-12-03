#pragma once

#include "CoreMinimal.h"
#include "PMCameraMode.generated.h"

UCLASS(Abstract, NotBlueprintable)
class PROJECTM_API UPMCameraMode : public UObject
{
	GENERATED_BODY()

/*
* Engine Functions
*/
public:
	UPMCameraMode();

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
public:
};

UCLASS()
class UPMCameraModeStack : public UObject
{
	GENERATED_BODY()

/*
* Engine Functions
*/
public:
	UPMCameraModeStack();

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
private:
	UPROPERTY()
	TArray<TObjectPtr<UPMCameraMode>> CameraModeInstance;

	UPROPERTY()
	TArray<TObjectPtr<UPMCameraMode>> CameraModeStack;
};