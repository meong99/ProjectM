#include "MInteractionActivity_Shop.h"
#include "MInteractionComponent.h"
#include "UI/MViewportClient.h"
#include "PMGameplayTags.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

UMInteractionActivity_Shop::UMInteractionActivity_Shop(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMInteractionActivity_Shop::InitAction(UMInteractionComponent* InInteractionComponent, AActor* InOwnerActor)
{
	Super::InitAction(InInteractionComponent, InOwnerActor);
}

void UMInteractionActivity_Shop::ActivateAction()
{
	UMViewportClient* ViewportClient = Cast<UMViewportClient>(GetWorld()->GetGameInstance()->GetGameViewportClient());
	if (ViewportClient)
	{
		bIsActivated = true;
		
		ViewportClient->RemoveWidgetFromLayer(FPMGameplayTags::Get().UI_Registry_Game_InteractionList);
		ViewportClient->AddWidgetToLayer(FPMGameplayTags::Get().UI_Registry_Game_Shop, {this, OwnerActor});
	}

	Super::ActivateAction();
}

void UMInteractionActivity_Shop::DeactivateAction()
{
	bIsActivated = false;
	UMViewportClient* ViewportClient = Cast<UMViewportClient>(GetWorld()->GetGameInstance()->GetGameViewportClient());
	if (ViewportClient && InteractionComponent)
	{
		ViewportClient->RemoveWidgetFromLayer(FPMGameplayTags::Get().UI_Registry_Game_Shop);
		InteractionComponent->ActivateAllOverlapAction();
	}
	Super::DeactivateAction();
}
