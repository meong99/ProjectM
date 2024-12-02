#pragma once

#include "Components/GameStateComponent.h"
#include "PMExperienceManagerComponent.generated.h"

enum class EPMExperienceLoadState
{
	Unloaded,
	Loading,
// 	LoadingGameFeatures,
// 	ExecutingActions,
	Loaded,
	Deactivating,
};

class UPMExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnExperienceLoaded, const UPMExperienceDefinition* /*LoadedExperienceDefinition*/);

UCLASS()
class PROJECTM_API UPMExperienceManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

/*
* Engine Functions
*/
public:
 UPMExperienceManagerComponent(const FObjectInitializer& ObjectInitializer);

/*
* Member Functions
*/
public:
	void SetCurrentExperience(FPrimaryAssetId ExperienceId);

	// 외부에서 Experience 데이터가 필요할 때 사용한다.
	// Experience Loading이 완료되어있다면 바로 Delegate를 Execute하고, 아직 덜 됐다면 Delegate를 저장해뒀다가 모두 Loading되면 그때 Execute해준다.
	void CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate&& Delegate);

	bool							IsExperienceLoaded() const { return LoadState == EPMExperienceLoadState::Loaded && CurrentExperience != nullptr; }
	const UPMExperienceDefinition*	GetCurrentExperienceChecked() const;
	EPMExperienceLoadState			GetLoadState() const { return LoadState; }

private:
	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnExperienceFullLoadCompleted();
/*
* Member Variables
*/
public:
	FOnExperienceLoaded OnExperienceLoaded;

private:
	UPROPERTY()
	TObjectPtr<const UPMExperienceDefinition> CurrentExperience;

	EPMExperienceLoadState LoadState = EPMExperienceLoadState::Unloaded;

	TArray<FString> GameFeaturePluginURLs;

	int32 NumGameFeaturePluginsLoading = 0;
};