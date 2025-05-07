#include "PMPlayerControllerBase.h"
#include "Camera/PMPlayerCameraManager.h"
#include "PMPlayerState.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "Engine/GameInstance.h"
#include "UI/MViewportClient.h"
#include "Inventory/PMInventoryManagerComponent.h"
#include "Components/MPlayerTradeComponent.h"

APMPlayerControllerBase::APMPlayerControllerBase()
{
	PlayerCameraManagerClass = APMPlayerCameraManager::StaticClass();
	InventoryManagerComponent = CreateDefaultSubobject<UPMInventoryManagerComponent>(TEXT("InventoryManagerComponent"));
	PlayerTradeComponent = CreateDefaultSubobject<UMPlayerTradeComponent>(TEXT("PlayerTradeComponent"));
	bReplicates = true;
}

void APMPlayerControllerBase::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	AMPlayerCharacterBase* MyCharacter = Cast<AMPlayerCharacterBase>(GetPawn());
	if (MyCharacter)
	{
		Delegate_OnPossessed.Broadcast(MyCharacter);
	}
}

void APMPlayerControllerBase::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UPMAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void APMPlayerControllerBase::CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate&& Delegate)
{
	if (GetWorld() && GetWorld()->GetGameState())
	{
		UPMExperienceManagerComponent* ExperienceManager = GetWorld()->GetGameState()->FindComponentByClass<UPMExperienceManagerComponent>();
		if (ExperienceManager)
		{
			ExperienceManager->CallOrRegister_OnExperienceLoaded(MoveTemp(Delegate));
		}
	}
}

void APMPlayerControllerBase::CallOrRegister_OnPossessed(FOnPossessed::FDelegate&& Delegate)
{
	AMPlayerCharacterBase* MyCharacter = Cast<AMPlayerCharacterBase>(GetPawn());
	if (MyCharacter)
	{
		Delegate.Execute(MyCharacter);
	}
	else
	{
		Delegate_OnPossessed.Add(MoveTemp(Delegate));
	}
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

void APMPlayerControllerBase::Debug_WidgetControl(const FGameplayTag& WidgetTag, bool bAddWidget, UObject* WidgetInstigator)
{
	UMViewportClient* VC = Cast<UMViewportClient>(GetGameInstance()->GetGameViewportClient());
	if (VC && bAddWidget)
	{
		VC->AddWidgetToLayer(WidgetTag, { WidgetInstigator, this});
	}
	else if (VC && !bAddWidget)
	{
		VC->RemoveWidgetFromLayer(WidgetTag);
	}
}
