#include "MInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MPlayerCharacterBase.h"
#include "Player/PMPlayerControllerBase.h"
#include "Input/PMInputComponent.h"
#include "PMGameplayTags.h"
#include "Util/MGameplayStatics.h"
#include "UI/MViewportClient.h"
#include "Util/MGameplayStatics.h"
#include "MInteractiveAction_OverlapActionBase.h"
#include "MInteractiveAction_OnInteractionBase.h"

UMInteractionComponent::UMInteractionComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
	SetCollisionProfileName(*UEnum::GetDisplayValueAsText(EMCollisionChannel::Interaction).ToString());
}

void UMInteractionComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OnComponentBeginOverlap.AddDynamic(this, &UMInteractionComponent::OnBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UMInteractionComponent::OnEndOverlap);
}

void UMInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (UMInteractiveAction_OverlapActionBase* Action : Action_OnBeginOverlap)
	{
		if (Action)
		{
			Action->InitAction(this, GetOwner());
		}
	}
	for (UMInteractiveAction_OnInteractionBase* Action : Action_OnInteract)
	{
		if (Action)
		{
			Action->InitAction(this, GetOwner());
		}
	}
}

void UMInteractionComponent::SetNewInteractions(const TArray<UMInteractiveAction_OverlapActionBase*>& OnBeginOverlap, const TArray<UMInteractiveAction_OnInteractionBase*>& OnInteract)
{
	Action_OnBeginOverlap.Empty();
	Action_OnInteract.Empty();
	for (UMInteractiveAction_OverlapActionBase* NewAction : OnBeginOverlap)
	{
		if (IsValid(NewAction))
		{
			Action_OnBeginOverlap.Add(NewAction);
		}
	}

	for (UMInteractiveAction_OnInteractionBase* NewAction : OnInteract)
	{
		if (IsValid(NewAction))
		{
			Action_OnInteract.Add(NewAction);
		}
	}
}

void UMInteractionComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (Controller && Controller->GetPawn() == OtherActor)
	{
		ActivateAllOverlapAction();
	}

	BindDelegate();
}

void UMInteractionComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (Controller && Controller->GetPawn() == OtherActor)
	{
		DeactivateAllOverlapAction();
	}

	UnbindDelegate();
}

void UMInteractionComponent::OnInteract(const FGameplayTag& Tag)
{
	UMViewportClient* ViewportClient = UMGameplayStatics::GetViewportClient(this);
	if (ViewportClient && Action_OnInteract.Num() > 0)
	{
		ViewportClient->AddWidgetToLayer(FPMGameplayTags::Get().UI_Registry_Game_InteractionList, { this, GetOwner() });
	}
}

void UMInteractionComponent::Callback_OnSetInputComponent(UInputComponent* InInputComponent)
{
	UPMInputComponent* InputComponent = Cast<UPMInputComponent>(InInputComponent);
	if (InputComponent)
	{
		InputComponent->InputActionDelegateMap.Emplace(FPMGameplayTags::Get().InputTag_Togle_Interaction, FInputActionDelegate::FDelegate::CreateUObject(this, &ThisClass::OnInteract));
	}
}

void UMInteractionComponent::BindDelegate()
{
	UPMInputComponent* InputComponent = GetInputComponent();
	if (InputComponent)
	{
		InputComponent->InputActionDelegateMap.Emplace(FPMGameplayTags::Get().InputTag_Togle_Interaction, FInputActionDelegate::FDelegate::CreateUObject(this, &ThisClass::OnInteract));
	}
}

void UMInteractionComponent::UnbindDelegate()
{
	UPMInputComponent* InputComponent = GetInputComponent();
	if (InputComponent)
	{
		InputComponent->InputActionDelegateMap.Remove(FPMGameplayTags::Get().InputTag_Togle_Interaction);
	}

	UMViewportClient* ViewportClient = UMGameplayStatics::GetViewportClient(this);
	if (ViewportClient)
	{
		ViewportClient->RemoveWidgetFromLayer(FPMGameplayTags::Get().UI_Registry_Game_InteractionList);
	}
}

void UMInteractionComponent::ActivateAllOverlapAction()
{
	for (UMInteractiveAction_Base* Action : Action_OnBeginOverlap)
	{
		if (Action)
		{
			Action->ActivateAction();
		}
	}
}

void UMInteractionComponent::DeactivateAllOverlapAction()
{
	for (UMInteractiveAction_Base* Action : Action_OnBeginOverlap)
	{
		if (Action)
		{
			Action->DeactivateAction();
		}
	}
}

UPMInputComponent* UMInteractionComponent::GetInputComponent() const
{
	APMPlayerControllerBase* Controller = Cast<APMPlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));
	if (Controller && Controller->GetPawn())
	{
		return Cast<UPMInputComponent>(Controller->GetPawn()->InputComponent);
	}

	return nullptr;
}
