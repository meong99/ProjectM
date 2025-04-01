#include "Character/MNpcBase.h"
#include "Components/WidgetComponent.h"

AMNpcBase::AMNpcBase()
{
	NpcName = CreateDefaultSubobject<UWidgetComponent>(TEXT("NpcName"));
}
