#include "MCircleIndicator.h"
#include "Materials/MaterialInstanceDynamic.h"

AMCircleIndicator::AMCircleIndicator()
{
}

void AMCircleIndicator::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AMCircleIndicator::BeginPlay()
{
	Super::BeginPlay();

	if (IndicatorInstance)
	{
		IndicatorInstance->SetScalarParameterValue(FName("InnerCircleRadius"), Radius);
		IndicatorInstance->SetVectorParameterValue(FName("IndicatorColor"), IndicatorColor);
	}
}
