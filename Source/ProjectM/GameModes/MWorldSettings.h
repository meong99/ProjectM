#pragma once

#include "GameFramework/WorldSettings.h"
#include "MWorldSettings.generated.h"

class UPMExperienceDefinition;

UCLASS()
class PROJECTM_API AMWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	AMWorldSettings(const FObjectInitializer& ObjectInitializer);

/*
* Member Functions
*/
public:
	FPrimaryAssetId GetDefaultGameplayExperience() const;

/*
* Member Variables
*/
private:
	UPROPERTY(EditDefaultsOnly, Category=GameMode)
	TSoftClassPtr<UPMExperienceDefinition> DefaultGameplayExperience;
};