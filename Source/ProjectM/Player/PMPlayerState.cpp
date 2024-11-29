#include "PMPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "../GameModes/PMExperienceManagerComponent.h"
#include "../GameModes/PMGameModeBase.h"
#include "../Character/PMPawnData.h"

APMPlayerState::APMPlayerState()
{
}

void APMPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	UPMExperienceManagerComponent* ExperienceManagerComp = GameState->FindComponentByClass<UPMExperienceManagerComponent>();
	check(ExperienceManagerComp);

	ExperienceManagerComp->CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void APMPlayerState::OnExperienceLoaded(const UPMExperienceDefinition* CurrentExperience)
{
	if (APMGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APMGameModeBase>())
	{
		const UPMPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}

void APMPlayerState::SetPawnData(const UPMPawnData* InPawnData)
{
	check(InPawnData);
	check(!PawnData);

	PawnData = InPawnData;
}