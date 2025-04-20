#include "MNpcBase.h"
#include "Interaction/MInteractionComponent.h"
#include "MNpcDefinition.h"

AMNpcBase::AMNpcBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (!IsRunningDedicatedServer())
	{
		InteractionComponent = CreateDefaultSubobject<UMInteractionComponent>(TEXT("InteractionComponent"));
		InteractionComponent->SetupAttachment(GetRootComponent());
	}
}

void AMNpcBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMNpcBase::InitCharacterName()
{
	if (NpcDefinition)
	{
		CharacterName = NpcDefinition->NpcName;
	}
}

void AMNpcBase::BeginPlay()
{
	Super::BeginPlay();
}
