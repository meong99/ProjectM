// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MViewportClient.h"
#include "DataAssets/PMAssetManager.h"
#include "WidgetRegister/MWidgetRegister.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "MWidgetInstanceList.h"
#include "UI/MWidgetBase.h"
#include "PMGameplayTags.h"
#include "UI/MWidgetLayout.h"

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

			ApplyWidgetLayout();
		})
	);
}

void UMViewportClient::AddWidgetRegister(const FGameplayTag& RegisterTag, UMWidgetRegister* InWidgetRegister)
{
	if (RegisterTag.IsValid() && InWidgetRegister)
	{
		RemoveWidgetRegister(RegisterTag);
		WidgetRegisterMap.Emplace(RegisterTag, InWidgetRegister);
	}
}

void UMViewportClient::RemoveWidgetRegister(const FGameplayTag& RegisterTag)
{
	UMWidgetRegister* Register = WidgetRegisterMap.FindRef(RegisterTag);
	if (Register)
	{
		for (const auto& Iter : Register->MappedWidgetData.WidgetData)
		{
			RemoveWidgetFromLayer(Iter.Key);
		}
	}
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

void UMViewportClient::AddWidgetToViewport(const FGameplayTag& WidgetTag)
{
	UMWidgetBase* Widget = GetWidgetInstance(WidgetTag);

	if (Widget && !Widget->IsInViewport())
	{
		Widget->AddToViewport();
	}
}

void UMViewportClient::RemoveWidgetFromParent(const FGameplayTag& WidgetTag)
{
	UMWidgetBase* Widget = GetWidgetInstance(WidgetTag);

	if (Widget && Widget->IsInViewport())
	{
		Widget->RemoveFromParent();
	}
}

void UMViewportClient::AddWidgetToLayer(const FGameplayTag& WidgetTag, const FMWidgetInfo& InWidgetInfo, const int32 LayerId)
{
	UMWidgetBase* Widget = GetWidgetInstance(WidgetTag);
	if (WidgetLayout && Widget)
	{
		Widget->SetWidgetInfo(InWidgetInfo);
		if (!Widget->IsInitialized())
		{
			Widget->SetInitialized(true);
			Widget->PreAddToLayer();
		}
		WidgetLayout->AddWidgetToLayer(GetWidgetInstance(WidgetTag), (EMWidgetLayout)LayerId);
	}
	else
	{
		MCHAE_WARNING("WidgetLayout or WidgetInstance is null!");
	}
}

void UMViewportClient::RemoveWidgetFromLayer(const FGameplayTag& WidgetTag, const int32 LayerId)
{
	UMWidgetBase* Widget = GetWidgetInstance(WidgetTag);
	if (WidgetLayout && Widget)
	{
		Widget->SetWidgetInfo({});
		WidgetLayout->RemoveWidgetFromLayer(GetWidgetInstance(WidgetTag), (EMWidgetLayout)LayerId);
		Widget->SetActivate(false);
	}
	else
	{
		MCHAE_WARNING("WidgetLayout or WidgetInstance is null!");
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

	// WidgetInstanceList에서 Widget찾기
	UMWidgetBase* NewWidget = nullptr;
	UMWidgetInstanceList* WidgetInstanceList = WidgetInstanceListMap.FindRef(WidgetRegister->RegisterTag);
	if (WidgetInstanceList)
	{
		NewWidget = WidgetInstanceList->GetWidgetInstance(WidgetTag);
		if (NewWidget)
		{
			return NewWidget;
		}
	}
	else
	{
		// WidgetList가 없다면 만들기
		WidgetInstanceList = CreateNewWidgetInstanceList(WidgetRegister->RegisterTag);
	}

	// List에 Widget이 없어서 새로 만들고 추가 후 리턴
	NewWidget = WidgetInstanceList->CreateNewWidget(WidgetTag, WidgetRegister->GetWidgetClass(WidgetTag));

	return NewWidget;
}

UMWidgetInstanceList* UMViewportClient::CreateNewWidgetInstanceList(const FGameplayTag& RegisterTag)
{
	UMWidgetInstanceList* WidgetInstanceList = UMWidgetInstanceList::CreateNewWidgetInstanceList(this, RegisterTag);
	WidgetInstanceListMap.Add(RegisterTag, WidgetInstanceList);

	return WidgetInstanceList;
}

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

void UMViewportClient::ApplyWidgetLayout()
{
	if (WidgetLayout)
	{
		if (!WidgetLayout->IsInViewport())
		{
			WidgetLayout->AddToViewport();
		}
		return;
	}

	UMWidgetRegister* WidgetRegister = GetWidgetRegister(FPMGameplayTags::Get().UI_Registry_Layout_DefaultLayout.RequestDirectParent());
	check(WidgetRegister);

	WidgetLayout = Cast<UMWidgetLayout>(GetWidgetInstance(FPMGameplayTags::Get().UI_Registry_Layout_DefaultLayout));
	if (WidgetLayout == nullptr)
	{
		MCHAE_WARNING("WidgetLayout is null");
		return;
	}

	WidgetLayout->AddToViewport();
}
