#include "MInteractionWidgetBase.h"

UMInteractionWidgetBase::UMInteractionWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMInteractionWidgetBase::Init(AActor* InInstigator)
{
	WidgetInstigator = InInstigator;
}
