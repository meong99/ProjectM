#pragma once

#include "Engine/DataAsset.h"
#include "PMUserFacingExperienceDefinition.generated.h"

UCLASS()
class PROJECTM_API UPMUserFacingExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

/*
* Engine Functions
*/
public:
	UPMUserFacingExperienceDefinition();

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, AllowedTypes = "Map"), Category = "ProjectM | Experience")
	FPrimaryAssetId MapId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true, AllowedTypes = "Map"), Category = "ProjectM | Experience")
	FPrimaryAssetId ExperienceId;
};