// Fill out your copyright notice in the Description page of Project Settings.

#include "CommonSessionSubSystem.h"
#include "Engine/AssetManager.h"
#include "Engine/LocalPlayer.h"
#include "AssetRegistry/AssetData.h"
#include "Engine/World.h"
#include "TimerManager.h"

/**
 * UCommonSession_HostSessionRequest
 */

FString UCommonSession_HostSessionRequest::ConstructTravelURL() const
{
	FString combinedExtraArgs;

	for (const auto& ExtraArg : ExtraArgs)
	{
		if (ExtraArg.Key.IsEmpty())
		{
			continue;
		}

		if (ExtraArg.Value.IsEmpty())
		{
			combinedExtraArgs += FString::Printf(TEXT("?%s"), *ExtraArg.Key);
		}
		else
		{
			combinedExtraArgs += FString::Printf(TEXT("?%s=%s"), *ExtraArg.Key, *ExtraArg.Value);
		}
	}

	return FString::Printf(TEXT("%s%s"), *URL, *combinedExtraArgs);
}

/**
 * UCommonSessionSubSystem
 */

void UCommonSessionSubSystem::HostSession(APlayerController* HostingPlayer, UCommonSession_HostSessionRequest* Request)
{
	ULocalPlayer* LocalPlayer = HostingPlayer != nullptr ? HostingPlayer->GetLocalPlayer() : nullptr;
	if (!LocalPlayer)
	{
		MCHAE_WARNING("LocalPlayer is null!");
		return;
	}

	Handle_TravelFailDelegate = GEngine->TravelFailureEvent.AddUObject(this, &UCommonSessionSubSystem::OnFailure_ClientTravel);
	Handle_NetworkDelegate = GEngine->OnNetworkFailure().AddUObject(this, &UCommonSessionSubSystem::OnFailureNetwork_ClientTravel);
	HostingPlayer->ClientTravel(Request->ConstructTravelURL(), ETravelType::TRAVEL_Absolute, true);
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, [this]()->void
		{
			//GEngine->TravelFailureEvent.Broadcast(GetWorld(), ETravelFailure::Type::ClientTravelFailure, TEXT("TimeOver"));
		}, 1, false);
}

void UCommonSessionSubSystem::OnFailure_ClientTravel(UWorld* World, ETravelFailure::Type FailType, const FString& ErrorString)
{
	MCHAE_WARNING("CientTravel on fail Because ", *ErrorString);
	GEngine->TravelFailureEvent.Remove(Handle_TravelFailDelegate);
}

void UCommonSessionSubSystem::OnFailureNetwork_ClientTravel(UWorld* World, UNetDriver* Driver, ENetworkFailure::Type FailType, const FString& ErrorString)
{
	MCHAE_WARNING("Network on fail Because ", *ErrorString);
	GEngine->TravelFailureEvent.Remove(Handle_NetworkDelegate);
}
