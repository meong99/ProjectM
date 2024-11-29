#pragma once

#include "PMExperienceDefinition.generated.h"

class UGameFeatureAction;
class UPMPawnData;

UCLASS()
class PROJECTM_API UPMExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

/*
* Engine Functions
*/
public:
	UPMExperienceDefinition();

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, Category = "ProjectM | Experience")
	TObjectPtr<UPMPawnData> DefaultPawnData;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectM | Experience")
	TArray<FString> GameFeatureToEnable;

	/*UPROPERTY(EditDefaultsOnly, Category = "Experience")
	TArray<TObjectPtr<UPMExperienceActionSet>> ActionSet;*/

	UPROPERTY(EditDefaultsOnly, Category = "ProjectM | Experience")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};