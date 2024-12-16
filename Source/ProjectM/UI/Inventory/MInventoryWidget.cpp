#include "MInventoryWidget.h"
#include "Input/PMInputComponent.h"
#include "PMGameplayTags.h"

UMInventoryWidget::UMInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UMInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		UPMInputComponent* PMInputComponent = PlayerController->FindComponentByClass<UPMInputComponent>();
		if (PMInputComponent)
		{
			PMInputComponent->ToggleInputActionMap.Add(FPMGameplayTags::Get().HUD_Slot_Inventory, FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnInput_ToggleInventory));
		}
	}

	DeactivateWidget();
}

void UMInventoryWidget::OnInput_ToggleInventory()
{
	if (IsActivated())
	{
		DeactivateWidget();
	}
	else
	{
		ActivateWidget();
	}
}
