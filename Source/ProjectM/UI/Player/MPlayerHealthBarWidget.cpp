#include "MPlayerHealthBarWidget.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Character/Components/PMHealthComponent.h"

UMPlayerHealthBarWidget::UMPlayerHealthBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMPlayerHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* Controller = GetOwningPlayer();
	if (Controller)
	{
		Controller->OnPossessedPawnChanged.AddDynamic(this, &UMPlayerHealthBarWidget::Callback_OnPossessedPawnChanged);
		Callback_OnPossessedPawnChanged(nullptr, Controller->GetPawn());
	}
}

void UMPlayerHealthBarWidget::Callback_OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (IsValid(OldPawn))
	{
		UPMHealthComponent* HealthComp = OldPawn->FindComponentByClass<UPMHealthComponent>();
		if (HealthComp)
		{
			HealthComp->OnHealthChanged.RemoveDynamic(this, &UMPlayerHealthBarWidget::Callback_OnHealthChanged);
		}
	}

	if (IsValid(NewPawn))
	{
		UPMHealthComponent* HealthComp = NewPawn->FindComponentByClass<UPMHealthComponent>();
		if (HealthComp)
		{
			HealthComp->CallOrRegister_OnInitHealthComponent(FOnInitHealth::FDelegate::CreateUObject(this, &UMPlayerHealthBarWidget::Callback_OnHealthChanged));
			HealthComp->OnHealthChanged.AddDynamic(this, &UMPlayerHealthBarWidget::Callback_OnHealthChanged);
		}
	}
}

void UMPlayerHealthBarWidget::Callback_OnHealthChanged(UPMHealthComponent* HealthComp, const float OldValue, const float NewValue, AActor* Instigator)
{
	BP_OnHealthChanged(HealthComp, OldValue, NewValue, Instigator);
}
