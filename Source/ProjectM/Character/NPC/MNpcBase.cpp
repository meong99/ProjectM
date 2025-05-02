#include "MNpcBase.h"
#include "Interaction/MInteractionComponent.h"
#include "MNpcDefinition.h"
#include "Components/CapsuleComponent.h"
#include "Util/MGameplayStatics.h"

AMNpcBase::AMNpcBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (!IsRunningDedicatedServer())
	{
		InteractionComponent = CreateDefaultSubobject<UMInteractionComponent>(TEXT("InteractionComponent"));
		InteractionComponent->SetupAttachment(GetRootComponent());
		InteractionComponent->SetSphereRadius(300.f);
	}

	GetCapsuleComponent()->SetCollisionProfileName(*UEnum::GetDisplayValueAsText(EMCollisionChannel::NPC).ToString());
	bReplicates = true;
}

void AMNpcBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMNpcBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMNpcBase::InitCharacterName()
{
	if (NpcDefinition)
	{
		CharacterName = NpcDefinition->NpcName;
	}
	Super::InitCharacterName();
}
