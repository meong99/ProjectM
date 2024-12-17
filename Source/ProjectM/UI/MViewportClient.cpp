// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MViewportClient.h"
#include "DataAssets/PMAssetManager.h"
#include "WidgetRegister/MWidgetRegister.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "CommonActivatableWidget.h"
#include "UIExtensionSystem.h"
#include "CommonUIExtensions.h"

UE_DISABLE_OPTIMIZATION
void UMViewportClient::Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice /*= true*/)
{
	Super::Init(WorldContext, OwningGameInstance, true);

	UPMAssetManager& AssetManager = UPMAssetManager::Get();
	check(UPMAssetManager::IsInitialized());

	FPrimaryAssetType		AssetType{ UMWidgetRegister::StaticClass()->GetFName() };
	TArray<FPrimaryAssetId>	PrimaryAssetIdList;
	AssetManager.GetPrimaryAssetIdList(AssetType, PrimaryAssetIdList);

	TArray<FName> LoadBundles;
	AssetManager.LoadPrimaryAssets(PrimaryAssetIdList, LoadBundles, FSimpleMulticastDelegate::FDelegate::CreateLambda(
		[this, PrimaryAssetIdList, &AssetManager]()->void
		{
			for (const FPrimaryAssetId& AssetId : PrimaryAssetIdList)
			{
				UMWidgetRegister* WidgetRegister = AssetManager.GetPrimaryAssetObject<UMWidgetRegister>(AssetId);

				if (IsValid(WidgetRegister) && WidgetRegister->RegisterTag.IsValid())
				{
					WidgetRegister->SetWorld(this);
					WidgetRegisterMap.Add(WidgetRegister->RegisterTag, WidgetRegister);
				}
				else
				{
					MCHAE_ERROR("WidgetRegistar or RegisterTag is not valid! Check!");
				}
			}
		})
	);
}

UMViewportClient* UMViewportClient::Get(const UObject* WorldContext)
{
	if (!WorldContext)
	{
		MCHAE_WARNING("WorldContextObject is null");
		return nullptr;
	}

	UWorld* World = WorldContext->GetWorld();
	if (World)
	{
		UGameInstance* GameInstance = World->GetGameInstance();
		if (GameInstance)
		{
			return Cast<UMViewportClient>(GameInstance->GetGameViewportClient());
		}
	}

	return nullptr;
}

void UMViewportClient::ActivateWidget(const FGameplayTag& RegisterTag, const FGameplayTag& WidgetTag)
{
	UUserWidget* Widget = GetWidgetInstance(RegisterTag, WidgetTag);

	if (UCommonActivatableWidget* ActivatableWidget = Cast<UCommonActivatableWidget>(Widget))
	{
		if (!ActivatableWidget->IsActivated())
		{
			ActivatableWidget->ActivateWidget();
		}
	}
	else if (Widget)
	{
		if (!Widget->IsInViewport())
		{
			Widget->AddToViewport();
		}
	}
}

UUserWidget* UMViewportClient::GetWidgetInstance(const FGameplayTag& RegisterTag, const FGameplayTag& WidgetTag)
{
	if (!RegisterTag.IsValid() || !WidgetTag.IsValid())
	{
		MCHAE_WARNING("RegisterTag or WidgetTag is not valid. RegisterTag = %s, WidgetTag = %s", *RegisterTag.ToString(), *WidgetTag.ToString());
		return nullptr;
	}

	UMWidgetRegister* WidgetRegister = WidgetRegisterMap.FindRef(RegisterTag);
	if (WidgetRegister == nullptr)
	{
		MCHAE_WARNING("Can't Found WidgetRegister! Check RegisterTag and WigetRegisterData");
		return nullptr;
	}

	UUserWidget* FoundWidget = WidgetRegister->GetWidgetInstanceAndLoadIfNotLoaded(WidgetTag);

	return FoundWidget;
}
UE_ENABLE_OPTIMIZATION
