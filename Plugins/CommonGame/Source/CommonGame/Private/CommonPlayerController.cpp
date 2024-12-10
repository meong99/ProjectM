// Fill out your copyright notice in the Description page of Project Settings.

#include "CommonPlayerController.h"
#include "CommonLocalPlayer.h"

void ACommonPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (UCommonLocalPlayer* LocalPlayer = Cast<UCommonLocalPlayer>(Player))
	{
		LocalPlayer->OnPlayerControllerSet.Broadcast(LocalPlayer, this);
		if (PlayerState)
		{
			LocalPlayer->OnPlayerStateSet.Broadcast(LocalPlayer, PlayerState);
		}
	}
}
