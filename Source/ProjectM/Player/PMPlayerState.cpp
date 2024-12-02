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
		// PlayerState에 있는 PawnData를 사용해도 되지만 없을수도 있고, 기본적으로 PawnData를 가져오는 곳을 하나로 통일해서 관리를 수월하게 하기 위해 GameMode에서 관리한다.
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