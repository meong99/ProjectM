// Fill out your copyright notice in the Description page of Project Settings.

#include "CommonSessionSubSystem.h"
#include "Engine/AssetManager.h"
#include "Engine/LocalPlayer.h"
#include "AssetRegistry/AssetData.h"
#include "Engine/World.h"

/**
 * UCommonSession_HostSessionRequest
 */

FString UCommonSession_HostSessionRequest::GetMapName() const
{
	FAssetData MapAssetData;
	if (UAssetManager::Get().GetPrimaryAssetData(MapId, MapAssetData))
	{
		return MapAssetData.PackageName.ToString();
	}

	return FString();
}

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

	return FString::Printf(TEXT("%s%s"), *GetMapName(), *combinedExtraArgs);
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

	GetWorld()->ServerTravel(Request->ConstructTravelURL());
}
