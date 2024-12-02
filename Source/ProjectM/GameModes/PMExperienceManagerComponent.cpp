#include "PMExperienceManagerComponent.h"
#include "../DataAssets/PMAssetManager.h"
#include "PMExperienceDefinition.h"
#include "../../GameFeatures/Source/GameFeatures/Public/GameFeaturesSubsystemSettings.h"
#include "../../GameFeatures/Source/GameFeatures/Public/GameFeaturesSubsystem.h"

UPMExperienceManagerComponent::UPMExperienceManagerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPMExperienceManagerComponent::SetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	UPMAssetManager& AssetManager = UPMAssetManager::Get();

	TSubclassOf<UPMExperienceDefinition> AssetClass;
	FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
	AssetClass = Cast<UClass>(AssetPath.TryLoad());

	const UPMExperienceDefinition* Experience = GetDefault<UPMExperienceDefinition>(AssetClass);
	check(Experience != nullptr);

	check(CurrentExperience == nullptr)
	CurrentExperience = Experience;

	StartExperienceLoad();
}

void UPMExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
}

void UPMExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == EPMExperienceLoadState::Unloaded);

	LoadState = EPMExperienceLoadState::Loading;

	UPMAssetManager& AssetManager = UPMAssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	TArray<FName> BundlesToLoad;
	const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
	bool bLoadClient = GIsEditor || OwnerNetMode != NM_DedicatedServer;
	bool bLoadServer = GIsEditor || OwnerNetMode != NM_Client;
	if (bLoadClient)
	{
		BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
	}
	if (bLoadServer)
	{
		BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);
	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{},
		false,
		FStreamableDelegate(),
		FStreamableManager::AsyncLoadHighPriority
	);

	if (Handle.IsValid() == false || Handle->HasLoadCompleted())
	{
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]() -> void
		{
			OnAssetsLoadedDelegate.ExecuteIfBound();	
		}));
	}
}

void UPMExperienceManagerComponent::OnExperienceLoadComplete()
{
	check(LoadState == EPMExperienceLoadState::Loading);
	check(CurrentExperience);

	OnExperienceFullLoadCompleted();
}

void UPMExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != EPMExperienceLoadState::Loaded);

	LoadState = EPMExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

const UPMExperienceDefinition* UPMExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == EPMExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);

	return CurrentExperience;
}
