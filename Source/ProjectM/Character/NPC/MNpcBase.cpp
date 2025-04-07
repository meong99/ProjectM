#include "MNpcBase.h"
#include "Character/Components/MNameWidgetComponent.h"
#include "Interaction/MInteractionComponent.h"

AMNpcBase::AMNpcBase()
{
	if (!IsRunningDedicatedServer())
	{
		NpcNameComp = CreateDefaultSubobject<UMNameWidgetComponent>(TEXT("NpcNameComp"));
		InteractionComponent = CreateDefaultSubobject<UMInteractionComponent>(TEXT("InteractionComponent"));
	}
}

void AMNpcBase::BeginPlay()
{
	Super::BeginPlay();
}
