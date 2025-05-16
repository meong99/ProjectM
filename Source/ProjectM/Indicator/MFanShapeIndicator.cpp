#include "MFanShapeIndicator.h"
#include "Materials/MaterialInstanceDynamic.h"

AMFanShapeIndicator::AMFanShapeIndicator()
{
}

void AMFanShapeIndicator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMFanShapeIndicator::BeginPlay()
{
	Super::BeginPlay();

	if (IndicatorInstance)
	{
		IndicatorInstance->SetScalarParameterValue(FName("Angle"), Angle);
		IndicatorInstance->SetVectorParameterValue(FName("IndicatorColor"), IndicatorColor);
	}
}
