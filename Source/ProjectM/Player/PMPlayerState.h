#pragma once

#include "GameFramework/PlayerState.h"
#include "PMPlayerState.generated.h"

class UPMExperienceDefinition;
class UPMPawnData;

UCLASS()
class PROJECTM_API APMPlayerState : public APlayerState
{
	GENERATED_BODY()

/*
* Engine Functions
*/
public:
	APMPlayerState();

	virtual void PostInitializeComponents() override;
	/*
* Member Functions
*/
public:
	void OnExperienceLoaded(const UPMExperienceDefinition* CurrentExperience);
	void SetPawnData(const UPMPawnData* InPawnData);
/*
* Member Variables
*/
private:
	//이미 Definition에 존재하지만, 빠르게 사용하기 위해 캐싱.
	UPROPERTY()
	TObjectPtr<const UPMPawnData> PawnData;
};