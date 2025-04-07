#include "MInteractionComponent.h"
#include "MInteractionActivity_Base.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameModes/PMExperienceManagerComponent.h"
#include "GameFramework/GameState.h"

UMInteractionComponent::UMInteractionComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InteractionShpere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCapsule"));

	bWantsInitializeComponent = true;
}

void UMInteractionComponent::InitializeComponent()
{
	Super::InitializeComponent();

	InteractionShpere->OnComponentBeginOverlap.AddDynamic(this, &UMInteractionComponent::OnBeginOverlap);
	InteractionShpere->OnComponentEndOverlap.AddDynamic(this, &UMInteractionComponent::OnEndOverlap);

	if (GetWorld()->GetGameState())
	{
		UPMExperienceManagerComponent* ExperienceManager = GetWorld()->GetGameState()->FindComponentByClass<UPMExperienceManagerComponent>();
		if (ExperienceManager)
		{
			ExperienceManager->CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate::CreateWeakLambda(this, [this](const UPMExperienceDefinition* LoadedExperienceDefinition)->void
				{
					for (UMInteractionActivity_Base* Action : Action_OnBeginOverlap)
					{
						if (Action)
						{
							Action->InitAction(this);
						}
					}
					for (UMInteractionActivity_Base* Action : Action_OnInteract)
					{
						if (Action)
						{
							Action->InitAction(this);
						}
					}
				}));
		}
	}
}

void UMInteractionComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (Controller && Controller->GetPawn() == OtherActor)
	{
		for (UMInteractionActivity_Base* Action : Action_OnBeginOverlap)
		{
			if (Action)
			{
				Action->ActivateAction();
			}
		}
	}
}

void UMInteractionComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	if (Controller && Controller->GetPawn() == OtherActor)
	{
		for (UMInteractionActivity_Base* Action : Action_OnBeginOverlap)
		{
			if (Action)
			{
				Action->DeactivateAction();
			}
		}
	}
}
