#pragma once

#include "Engine/DataAsset.h"
#include "PMPawnData.generated.h"

class UPMCameraMode;
class UPMInputConfig;
class UPMAbilitySet;

UCLASS()
class PROJECTM_API UPMPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
/*
* Overrided Function
*/
public:
	UPMPawnData();
	
/*
* Member Functions
*/
public:
	const TSubclassOf<APawn>&					GetPawnClass() const { return PawnClass; }
	const TSubclassOf<UPMCameraMode>&			GetDefaultCameraMode() const { return DefaultCameraMode; }
	const UPMInputConfig*						GetInputConfig() const { return InputConfig; }
	const TArray<TObjectPtr<UPMAbilitySet>>&	GetAbilitySets() const { return AbilitySets; }

/*
* Member Variables
*/
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ProjectM | Pawn")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ProjectM | Camera")
	TSubclassOf<UPMCameraMode> DefaultCameraMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ProjectM | Input")
	TObjectPtr<UPMInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "ProjectM | Abilities")
	TArray<TObjectPtr<UPMAbilitySet>> AbilitySets;
};