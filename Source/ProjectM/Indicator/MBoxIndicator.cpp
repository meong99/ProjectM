#include "MBoxIndicator.h"
#include "Materials/MaterialInstanceDynamic.h"

AMBoxIndicator::AMBoxIndicator()
{
}

void AMBoxIndicator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMBoxIndicator::BeginPlay()
{
	Super::BeginPlay();

	if (IndicatorInstance)
	{
		IndicatorInstance->SetScalarParameterValue(FName("LineThickness"), LineThickness);
		IndicatorInstance->SetVectorParameterValue(FName("IndicatorColor"), IndicatorColor);
	}
}
