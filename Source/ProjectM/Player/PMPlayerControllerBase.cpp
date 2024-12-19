#include "PMPlayerControllerBase.h"
#include "Camera/PMPlayerCameraManager.h"
#include "PMPlayerState.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"

APMPlayerControllerBase::APMPlayerControllerBase()
{
	PlayerCameraManagerClass = APMPlayerCameraManager::StaticClass();
}

void APMPlayerControllerBase::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UPMAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

APMPlayerState* APMPlayerControllerBase::GetPlayerState() const
{
	return Cast<APMPlayerState>(PlayerState);
}

UPMAbilitySystemComponent* APMPlayerControllerBase::GetAbilitySystemComponent() const
{
	const APMPlayerState* PMPlayerState = GetPlayerState();
	
	return PMPlayerState ? PMPlayerState->GetPMAbilitySystemComponent() : nullptr;
}
