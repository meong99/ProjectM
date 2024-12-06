#pragma once

#include "CoreMinimal.h"
#include "PMWeaponInstance.h"
#include "../Cosmetics/PMCosmeticAnimationTypes.h"
#include "PMRangedWeaponInstance.generated.h"

/**
 *
 */
UCLASS()
class PROJECTM_API UPMRangedWeaponInstance : public UPMWeaponInstance
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMRangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
private:
};