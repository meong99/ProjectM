#include "MInteractionActivity_Base.h"
#include "MInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PMPlayerControllerBase.h"

UMInteractionActivity_Base::UMInteractionActivity_Base(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMInteractionActivity_Base::InitAction(UMInteractionComponent* InInteractionComponent, AActor* InOwnerActor)
{
	InteractionComponent = InInteractionComponent;
	OwnerActor = InOwnerActor;
	K2_InitAction(InteractionComponent, OwnerActor, Cast<APMPlayerControllerBase>(UGameplayStatics::GetPlayerController(InteractionComponent, 0)));
}

void UMInteractionActivity_Base::ActivateAction()
{
	K2_ActivateAction();
}

void UMInteractionActivity_Base::DeactivateAction()
{
	K2_DeactivateAction();
}
