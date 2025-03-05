#pragma once

#include "GameFramework/GameStateBase.h"
#include "PMGameStateBase.generated.h"

class UPMExperienceManagerComponent;
class USaveGame;

UCLASS()
class PROJECTM_API APMGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	APMGameStateBase();
	virtual void BeginPlay() override;

/*
* Member Functions
*/
public:
	UPMExperienceManagerComponent* GetExperienceManagerComponent() const { return ExperienceManagerComponent; }
	
	UFUNCTION(Server, Reliable)
	void Server_SaveGame();
	UFUNCTION(Server, Reliable)
	void Server_LoadGame();

/*
* Member Variables
*/
private:
	UPROPERTY()
	UPMExperienceManagerComponent* ExperienceManagerComponent;
	
	UPROPERTY()
	TArray<TObjectPtr<USaveGame>> PlayerSaveDataArray;
};