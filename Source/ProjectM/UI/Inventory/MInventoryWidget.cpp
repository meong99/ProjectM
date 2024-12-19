#include "MInventoryWidget.h"
#include "Input/PMInputComponent.h"
#include "GameFramework/Pawn.h"

UE_DISABLE_OPTIMIZATION
UMInventoryWidget::UMInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UMInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		APawn* Pawn = PlayerController->GetPawn();
		if (Pawn)
		{
			OnPossessedPawn(nullptr, Pawn);
		}
		else
		{
			PlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawn);
		}
	}
}

void UMInventoryWidget::OnInput_ToggleInventory()
{
	if (IsInViewport())
	{
		RemoveFromParent();
	}
	else
	{
		AddToViewport();
	}
}

void UMInventoryWidget::OnPossessedPawn(APawn* OldPawn, APawn* NewPawn)
{
	if (!NewPawn)
	{
		return;
	}

	UPMInputComponent* PMInputComponent = Cast<UPMInputComponent>(NewPawn->InputComponent);
	if (PMInputComponent)
	{
		PMInputComponent->ToggleInputActionMap.Add(WidgetTag, FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnInput_ToggleInventory));
	}
}
UE_ENABLE_OPTIMIZATION
