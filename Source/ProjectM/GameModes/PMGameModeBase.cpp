#include "PMGameModeBase.h"

APMGameModeBase::APMGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void APMGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
}

void APMGameModeBase::InitGameState()
{
}

UClass* APMGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	return nullptr;
}

void APMGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
}

APawn* APMGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	return nullptr;
}
