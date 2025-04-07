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

void UMNameWidgetComponent::EnableNameWidget()
{
	if (NameWidget)
	{
		SetTickMode(ETickMode::Enabled);
		NameWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UMNameWidgetComponent::DisableNameWidget()
{
	if (NameWidget)
	{
		SetTickMode(ETickMode::Disabled);
		NameWidget->SetVisibility(ESlateVisibility::Hidden);
	}
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
