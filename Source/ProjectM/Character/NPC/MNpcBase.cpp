#include "MNpcBase.h"
#include "Character/Components/MNameWidgetComponent.h"

AMNpcBase::AMNpcBase()
{
	if (!IsRunningDedicatedServer())
	{
		NpcNameComp = CreateDefaultSubobject<UMNameWidgetComponent>(TEXT("NpcNameComp"));
	}
}

void AMNpcBase::BeginPlay()
{
	Super::BeginPlay();
}
