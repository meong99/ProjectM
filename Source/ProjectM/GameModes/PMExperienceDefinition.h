#pragma once

#include "Engine/DataAsset.h"
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
public:
	UPROPERTY(EditDefaultsOnly, Category = "ProjectM | Experience")
	TArray<FString> GameFeaturesToEnable;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ProjectM | Experience")
	TObjectPtr<UPMPawnData> DefaultPawnData;

	/*UPROPERTY(EditDefaultsOnly, Category = "Experience")
	TArray<TObjectPtr<UPMExperienceActionSet>> ActionSet;*/

	UPROPERTY(EditDefaultsOnly, Category = "ProjectM | Experience")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};