// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/GameModeBase.h"

#include "PMGameModeBase.generated.h"

UCLASS(Config = Game)
class APMGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	APMGameModeBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void	InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void	InitGameState() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual void	HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) final;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) final;

public:
	/*bool IsExperienceLoaded() const;
	const UDDPawnData* GetPawnDataForController(const AController* InController) const;*/

protected:
	//void HandleMatchAssignmentIfNotExpectingOne();
	//void OnMatchAssignmentGiven(const FPrimaryAssetId& ExperienceId);
	//void OnExperienceLoaded(const UDDExperienceDefinition* CurrentExperience);
};
