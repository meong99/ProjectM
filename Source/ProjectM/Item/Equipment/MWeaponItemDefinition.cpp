#include "MWeaponItemDefinition.h"
#include "Weapons/PMWeaponInstance.h"

UMWeaponItemDefinition::UMWeaponItemDefinition()
{
	InstanceType = UPMWeaponInstance::StaticClass();
}
