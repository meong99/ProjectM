#include "MRangeIndicatorActor.h"
#include "Components/DecalComponent.h"

AMRangeIndicatorActor::AMRangeIndicatorActor()
{
	IndicatorDecal = CreateDefaultSubobject<UDecalComponent>("IndicatorDecal");
	IndicatorDecal->SetRelativeRotation(FRotator(-90, 0, 0));
}

void AMRangeIndicatorActor::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	IndicatorInstance = IndicatorDecal->CreateDynamicMaterialInstance();
}

void AMRangeIndicatorActor::SetIndicatorColor(FColor InColor)
{
	IndicatorColor = InColor;
}
