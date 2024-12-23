// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MViewportClient.h"
#include "DataAssets/PMAssetManager.h"
#include "WidgetRegister/MWidgetRegister.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "MWidgetInstanceList.h"
#include "MWidgetBase.h"
#include "PMGameplayTags.h"
#include "UI/MWidgetLayout.h"

UE_DISABLE_OPTIMIZATION
void UMViewportClient::Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice /*= true*/)
{
	Super::Init(WorldContext, OwningGameInstance, true);
	LoadDefaultWidgetRegister();
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

void UMViewportClient::LoadDefaultWidgetRegister()
{
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
					AddWidgetRegister(WidgetRegister->RegisterTag, WidgetRegister);
				}
				else
				{
					MCHAE_ERROR("WidgetRegistar or RegisterTag is not valid! Check!");
				}
			}

			OnLoaded_DefaultWidgetRegister();
		})
	);
}

void UMViewportClient::AddWidgetRegister(const FGameplayTag& RegisterTag, UMWidgetRegister* InWidgetRegister)
{
	if (RegisterTag.IsValid() && InWidgetRegister)
	{
		WidgetRegisterMap.Emplace(RegisterTag, InWidgetRegister);
	}
}

void UMViewportClient::RemoveWidgetRegister(const FGameplayTag& RegisterTag)
{
	WidgetRegisterMap.Remove(RegisterTag);
	WidgetInstanceListMap.Remove(RegisterTag);
}

UMWidgetInstanceList* UMViewportClient::CreateWidgetInRegister(const FGameplayTag& RegisterTag)
{
	UMWidgetInstanceList* WidgetInstanceList = WidgetInstanceListMap.FindRef(RegisterTag);
	if (WidgetInstanceList == nullptr)
	{
		WidgetInstanceList = CreateNewWidgetInstanceList(RegisterTag);
		if (WidgetInstanceList)
		{
			WidgetInstanceList->CreateNewWidgets(WidgetRegisterMap.FindRef(RegisterTag));
		}
	}

	return WidgetInstanceList;
}

void UMViewportClient::AddWidgetToLayer(const FGameplayTag& WidgetTag)
{
	UMWidgetBase* Widget = GetWidgetInstance(WidgetTag);

	if (Widget && !Widget->IsInViewport())
	{
		Widget->AddToViewport();
	}
}

void UMViewportClient::RemoveWidgetFromLayer(const FGameplayTag& WidgetTag)
{
	UMWidgetBase* Widget = GetWidgetInstance(WidgetTag);

	if (Widget && Widget->IsInViewport())
	{
		Widget->RemoveFromParent();
	}
}

UMWidgetBase* UMViewportClient::GetWidgetInstance(const FGameplayTag& WidgetTag)
{
	if (!WidgetTag.IsValid())
	{
		MCHAE_WARNING("RegisterTag or WidgetTag is not valid. WidgetTag = %s", *WidgetTag.ToString());
		return nullptr;
	}

	UMWidgetRegister* WidgetRegister = GetWidgetRegister(WidgetTag);

	if (WidgetRegister == nullptr)
	{
		MCHAE_WARNING("Can't Found WidgetRegister! Check WidgetTag's currect");
		return nullptr;
	}

	UMWidgetInstanceList* WidgetInstanceList = WidgetInstanceListMap.FindRef(WidgetRegister->RegisterTag);
	if (WidgetInstanceList)
	{
		return WidgetInstanceList->GetWidgetInstance(WidgetTag);
	}

	WidgetInstanceList = CreateNewWidgetInstanceList(WidgetRegister->RegisterTag);
	WidgetInstanceListMap.Add(WidgetTag.RequestDirectParent(), WidgetInstanceList);

	UMWidgetBase* NewWidget = WidgetInstanceList->CreateNewWidget(WidgetTag, WidgetRegister->GetWidgetClass(WidgetTag));

	return NewWidget;
}

UMWidgetInstanceList* UMViewportClient::CreateNewWidgetInstanceList(const FGameplayTag& RegisterTag)
{
	return UMWidgetInstanceList::CreateNewWidgetInstanceList(this, RegisterTag);
}

UE_ENABLE_OPTIMIZATION

UMWidgetRegister* UMViewportClient::GetWidgetRegister(const FGameplayTag& Tag)
{
	UMWidgetRegister* WidgetRegister = nullptr;
	FGameplayTagContainer ParentTags = Tag.GetGameplayTagParents();
	for (const FGameplayTag& ParentTag : ParentTags)
	{
		if (ParentTag.IsValid())
		{
			WidgetRegister = WidgetRegisterMap.FindRef(ParentTag);

			if (WidgetRegister)
			{
				break;
			}
		}
	}

	return WidgetRegister;
}

void UMViewportClient::OnLoaded_DefaultWidgetRegister()
{
	ApplyWidgetLayout();
}

void UMViewportClient::ApplyWidgetLayout()
{
	if (WidgetLayout)
	{
		WidgetLayout->AddToViewport();
		return;
	}

	UMWidgetRegister* WidgetRegister = GetWidgetRegister(FPMGameplayTags::Get().UI_Registry_HUD_Layout.RequestDirectParent());
	check(WidgetRegister);

	WidgetLayout = Cast<UMWidgetLayout>(GetWidgetInstance(FPMGameplayTags::Get().UI_Registry_HUD_Layout));
	check(WidgetLayout);

	WidgetLayout->AddToViewport();
}
