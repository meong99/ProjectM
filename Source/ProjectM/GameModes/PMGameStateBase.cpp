#include "PMGameStateBase.h"
#include "PMExperienceManagerComponent.h"

APMGameStateBase::APMGameStateBase()
{
	ExperienceManagerComponent = CreateDefaultSubobject<UPMExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
