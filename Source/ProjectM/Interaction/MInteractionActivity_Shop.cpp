#include "MInteractionActivity_Shop.h"
#include "MInteractionComponent.h"

UMInteractionActivity_Shop::UMInteractionActivity_Shop(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMInteractionActivity_Shop::InitAction(UMInteractionComponent* InOwner)
{
	Super::InitAction(InOwner);
}

void UMInteractionActivity_Shop::ActivateAction()
{
	Super::ActivateAction();
	bIsActivated = true;
}

void UMInteractionActivity_Shop::DeactivateAction()
{
	Super::DeactivateAction();

	bIsActivated = false;
}
