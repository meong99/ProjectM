#include "PMGameModeBase.h"
#include "../Character/PMCharacterBase.h"
#include "PMGameStateBase.h"
#include "../Player/PMPlayerControllerBase.h"
#include "../Player/PMPlayerState.h"

APMGameModeBase::APMGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameStateClass = APMGameStateBase::StaticClass();
	DefaultPawnClass = APMCharacterBase::StaticClass();
	PlayerControllerClass = APMPlayerControllerBase::StaticClass();
	PlayerStateClass = APMPlayerState::StaticClass();
}

void APMGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void APMGameModeBase::InitGameState()
{
	Super::InitGameState();
}

UClass* APMGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void APMGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

APawn* APMGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}
