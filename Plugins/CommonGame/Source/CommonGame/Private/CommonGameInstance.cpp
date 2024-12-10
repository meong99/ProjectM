// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonGameInstance.h"
#include "CommonLocalPlayer.h"
#include "GameUIManagerSubsystem.h"

int32 UCommonGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	// GameInstance에 관리하는 Player 컨테이너에 추가한다. (ReturnVal은 새로 추가된 index)
	int32 ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);
	if (ReturnVal != INDEX_NONE)
	{
		// PrimaryPlayer는 처음만 캐싱
		if (!PrimaryPlayer.IsValid())
		{
			PrimaryPlayer = NewPlayer;
		}

		// UGameUIManagerSubsystem를 통해 NotifyPlayerAdded() 호출로 GameLayout을 추가한다.
		GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerAdded(Cast<UCommonLocalPlayer>(NewPlayer));
	}

	return ReturnVal;
}

bool UCommonGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	if (PrimaryPlayer == ExistingPlayer)
	{
		PrimaryPlayer.Reset();
	}

	GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerDestroyed(Cast<UCommonLocalPlayer>(ExistingPlayer));
	return Super::RemoveLocalPlayer(ExistingPlayer);
}
