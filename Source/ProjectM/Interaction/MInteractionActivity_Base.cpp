#include "MInteractionActivity_Base.h"
#include "MInteractionComponent.h"

UMInteractionActivity_Base::UMInteractionActivity_Base(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMInteractionActivity_Base::InitAction(UMInteractionComponent* InOwner)
{
	Owner = InOwner;
}
