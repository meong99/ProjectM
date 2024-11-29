#include "PMGameModeBase.h"
#include "../Character/PMCharacterBase.h"
#include "PMGameStateBase.h"
#include "../Player/PMPlayerControllerBase.h"
#include "../Player/PMPlayerState.h"
#include "PMExperienceDefinition.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PMExperienceManagerComponent.h"
#include "../Character/PMPawnData.h"
#include "TimerManager.h"

APMGameModeBase::APMGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GameStateClass = APMGameStateBase::StaticClass();
	DefaultPawnClass = APMCharacterBase::StaticClass();
	PlayerControllerClass = APMPlayerControllerBase::StaticClass();
	PlayerStateClass = APMPlayerState::StaticClass();
}

void APMGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void APMGameModeBase::InitGameState()
{
	Super::InitGameState();

	UPMExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UPMExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

bool APMGameModeBase::IsExperienceLoaded() const
{
	return false;
}

const UPMPawnData* APMGameModeBase::GetPawnDataForController(const AController* InController) const
{
	return nullptr;
}

void APMGameModeBase::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;
	UWorld* World = GetWorld();

	// Option으로 넘겨준 값을 이용해 ExperienceDefinition을 찾는다.
	if (ExperienceId.IsValid() == false && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UPMExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromOptions));
	}

	// Experience를 마지막까지 못 찾는다면 기본값을 사용한다.
	if (ExperienceId.IsValid() == false)
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(FPrimaryAssetType("PMExperienceDefinition")), FName("BP_DefautExperienceDefinition"));
	}

	OnMatchAssignmentGiven(ExperienceId);
}

void APMGameModeBase::OnMatchAssignmentGiven(const FPrimaryAssetId& ExperienceId)
{
	check(ExperienceId.IsValid());
	check(GameState);

	UPMExperienceManagerComponent* ExperienceManagerComp = GameState->FindComponentByClass<UPMExperienceManagerComponent>();
	check(ExperienceManagerComp);

	ExperienceManagerComp->SetCurrentExperience(ExperienceId);
}

void APMGameModeBase::OnExperienceLoaded(const UPMExperienceDefinition* CurrentExperience)
{
}

UClass* APMGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void APMGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

APawn* APMGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}
