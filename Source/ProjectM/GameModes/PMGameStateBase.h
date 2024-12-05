#pragma once

#include "GameFramework/GameStateBase.h"
#include "PMGameStateBase.generated.h"

class UPMExperienceManagerComponent;

UCLASS()
class PROJECTM_API APMGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	APMGameStateBase();
/*
* Member Functions
*/
public:
	UPMExperienceManagerComponent* GetExperienceManagerComponent() const { return ExperienceManagerComponent; }

/*
* Member Variables
*/
private:
	UPROPERTY()
	UPMExperienceManagerComponent* ExperienceManagerComponent;
};