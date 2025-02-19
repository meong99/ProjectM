#include "PMGameStateBase.h"
#include "PMExperienceManagerComponent.h"
#include "Player/PMPlayerState.h"

APMGameStateBase::APMGameStateBase()
{
	ExperienceManagerComponent = CreateDefaultSubobject<UPMExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}

void APMGameStateBase::Server_SaveGame_Implementation()
{
	for (APlayerState* PlayerState : PlayerArray)
	{
		APMPlayerState* MyPlayerState = Cast<APMPlayerState>(PlayerState);
		if (MyPlayerState)
		{
			MyPlayerState->Server_SavePlayerData();
		}
	}
}

void APMGameStateBase::Server_LoadGame_Implementation()
{
	for (APlayerState* PlayerState : PlayerArray)
	{
		APMPlayerState* MyPlayerState = Cast<APMPlayerState>(PlayerState);
		if (MyPlayerState)
		{
			MyPlayerState->Server_LoadPlayerData();
		}
	}
}
