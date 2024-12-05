#pragma once

#include "Engine/DataAsset.h"
#include "PMExperienceActionSet.generated.h"

class UGameFeatureAction;

UCLASS()
class PROJECTM_API UPMExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UPMExperienceActionSet();

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
public:

	UPROPERTY(EditAnywhere, Category = "ProjectM | ActionsToPerform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};