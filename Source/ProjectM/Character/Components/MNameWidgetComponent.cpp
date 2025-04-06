#include "Character/Components/MNameWidgetComponent.h"
#include "Character/NPC/MNpcBase.h"
#include "UI/Name/MNameWidget.h"
#include "Character/NPC/MNpcDefinition.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

UE_DISABLE_OPTIMIZATION

UMNameWidgetComponent::UMNameWidgetComponent()
{
	bWantsInitializeComponent = true;
}

#pragma TODO("이거 Tick말고 beginoverlap으로 바꿔")
void UMNameWidgetComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (WeakPlayerPawn.IsValid() && WeakOwnerCharacter.IsValid() && NameWidget)
	{
		FVector PlayerLocation = WeakPlayerPawn->GetActorLocation();
		FVector OwnerLocation = WeakOwnerCharacter->GetActorLocation();
		double Dist = FVector::Dist(PlayerLocation, OwnerLocation);
		if (Dist > MaxDist)
		{
			NameWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (NameWidget->GetVisibility() == ESlateVisibility::Hidden)
		{
			NameWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else if (!WeakPlayerPawn.IsValid())
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			WeakPlayerPawn = PlayerController->GetPawn();
		}
	}
}

void UMNameWidgetComponent::InitWidget()
{
	Super::InitWidget();
	NameWidget = Cast<UMNameWidget>(GetWidget());
	WeakOwnerCharacter = Cast<ACharacter>(GetOwner());
	if (WeakOwnerCharacter.IsValid())
	{
		InitNameWidget();
		AdjustNameWidgetCompLocation();
	}
}

void UMNameWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMNameWidgetComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UMNameWidgetComponent::InitNameWidget()
{
	if (NameWidget && WeakOwnerCharacter.IsValid())
	{
		FName Name;
		if (AMNpcBase* Npc = Cast<AMNpcBase>(WeakOwnerCharacter.Get()))
		{
			UMNpcDefinition* NpcDefinition = Npc->GetNpcDefinition();
			if (NpcDefinition)
			{
				Name = NpcDefinition->NpcName;
			}
		}
		else if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
		{
			APlayerState* State = PlayerController->GetPlayerState<APlayerState>();
			if (State)
			{
				Name = *State->GetPlayerName();
			}
		}

		NameWidget->SetName(Name);
	}
}

void UMNameWidgetComponent::AdjustNameWidgetCompLocation()
{
	UCapsuleComponent* Capsule = WeakOwnerCharacter->GetCapsuleComponent();
	if (Capsule)
	{
		float Height = Capsule->GetScaledCapsuleHalfHeight();
		SetRelativeLocation(WeakOwnerCharacter->GetActorLocation() + FVector::UpVector * (Height * 1.5));
	}
}

UE_ENABLE_OPTIMIZATION
