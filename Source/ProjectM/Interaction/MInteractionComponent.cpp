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
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Character/Monster/MMonsterBase.h"

UMInteractionComponent::UMInteractionComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
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

	if (!IsItServer())
	{
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
}

void UMInteractionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (WeakOverlappedCharacter.IsValid())
	{
		if (FVector::Dist(WeakOverlappedCharacter->GetActorLocation(), GetComponentLocation()) > SphereRadius * 2)
		{
			DisableInteraction(WeakOverlappedCharacter.Get());
		}
	}
	else
	{
		DisableInteraction(nullptr);
	}
}

void UMInteractionComponent::SetNewInteractions(const TArray<UMInteractiveAction_OverlapActionBase*>& OnBeginOverlap, const TArray<UMInteractiveAction_OnInteractionBase*>& OnInteract)
{
	if (!IsItServer())
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
}

void UMInteractionComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor->GetLocalRole() == ENetRole::ROLE_SimulatedProxy)
	{
		return;
	}

	EnableInteraction(OtherActor);
}

void UMInteractionComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor || OtherActor->GetLocalRole() == ENetRole::ROLE_SimulatedProxy)
	{
		return;
	}

	DisableInteraction(OtherActor);
}

void UMInteractionComponent::EnableInteraction(AActor* OtherActor)
{
	AMPlayerCharacterBase* Character = Cast<AMPlayerCharacterBase>(OtherActor);
	if (Character)
	{
		if (GetOwner()->IsA(AMMonsterBase::StaticClass()))
		{
			Character->AddOverlappedMonster(GetOwner());
		}
		WeakOverlappedCharacter = Character;
	}

	if (!IsItServer())
	{
		ActivateAllOverlapAction();
		SetComponentTickEnabled(true);

		BindDelegate();
	}
}

void UMInteractionComponent::DisableInteraction(AActor* OtherActor)
{
	AMPlayerCharacterBase* Character = Cast<AMPlayerCharacterBase>(OtherActor);
	if (Character)
	{
		if (GetOwner()->IsA(AMMonsterBase::StaticClass()))
		{
			Character->RemoveOverlappedMonster(GetOwner());
		}
	}

	WeakOverlappedCharacter = nullptr;
	if (!IsItServer())
	{
		DeactivateAllOverlapAction();
		SetComponentTickEnabled(false);

		UnbindDelegate();
	}
}

void UMInteractionComponent::OnInteract(const FGameplayTag& Tag)
{
	UMViewportClient* ViewportClient = UMGameplayStatics::GetViewportClient(this);
	if (ViewportClient && ExistActivatableAction())
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

bool UMInteractionComponent::IsItServer() const
{
	return GetNetMode() == ENetMode::NM_DedicatedServer;
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

bool UMInteractionComponent::ExistActivatableAction() const
{
	bool bExistActivatableAction = false;
	if (Action_OnInteract.Num() == 0)
	{
		bExistActivatableAction = false;
		return bExistActivatableAction;
	}

	for (UMInteractiveAction_OnInteractionBase* Action : Action_OnInteract)
	{
		if (Action && Action->ShouldActivate())
		{
			bExistActivatableAction = true;
			break;
		}
	}

	return bExistActivatableAction;
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
