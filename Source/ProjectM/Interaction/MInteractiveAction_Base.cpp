#include "MInteractiveAction_Base.h"
#include "MInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PMPlayerControllerBase.h"
#include "PMGameplayTags.h"
#include "UI/MViewportClient.h"
#include "Util/MGameplayStatics.h"

UMInteractiveAction_Base::UMInteractiveAction_Base(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMInteractiveAction_Base::InitAction(UMInteractionComponent* InInteractionComponent, AActor* InOwnerActor)
{
	InteractionComponent = InInteractionComponent;
	OwnerActor = InOwnerActor;
	K2_InitAction(InteractionComponent, OwnerActor, Cast<APMPlayerControllerBase>(UGameplayStatics::GetPlayerController(InteractionComponent, 0)));
}

void UMInteractiveAction_Base::ActivateAction()
{
	UMViewportClient* ViewportClient = UMGameplayStatics::GetViewportClient(this);
	if (ViewportClient)
	{
		ViewportClient->RemoveWidgetFromLayer(FPMGameplayTags::Get().UI_Registry_Game_InteractionList);
	}
	K2_ActivateAction();
}

void UMInteractiveAction_Base::DeactivateAction()
{
	K2_DeactivateAction();
}
