#pragma once

#include "Engine/DataAsset.h"
#include "PMPawnData.generated.h"

UCLASS()
class PROJECTM_API UPMPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
/*
* Engine Functions
*/
public:
	UPMPawnData();
	
/*
* Member Functions
*/
public:

/*
* Member Variables
*/
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ProjectM | Pawn")
	TSubclassOf<APawn> PawnClass;

// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ProjectM | Camera")
// 	TSubclassOf<UPMCameraMode> DefaultCameraMode;

// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ProjectM | Input")
// 	TObjectPtr<UPMInputConfig> InputConfig;
};