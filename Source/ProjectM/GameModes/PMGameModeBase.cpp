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
