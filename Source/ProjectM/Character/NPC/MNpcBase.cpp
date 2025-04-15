#include "MNpcBase.h"
#include "Character/Components/MNameWidgetComponent.h"
#include "Interaction/MInteractionComponent.h"
#include "Components/SphereComponent.h"

AMNpcBase::AMNpcBase()
{
	if (!IsRunningDedicatedServer())
	{
		NpcNameComp = CreateDefaultSubobject<UMNameWidgetComponent>(TEXT("NpcNameComp"));
		InteractionComponent = CreateDefaultSubobject<UMInteractionComponent>(TEXT("InteractionComponent"));
		InteractionShpere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCapsule"));
	}
}

void AMNpcBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	InteractionShpere->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void AMNpcBase::BeginPlay()
{
	Super::BeginPlay();
}
