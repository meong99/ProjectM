#pragma once

#include "CoreMinimal.h"
#include "MEquipmentItemDefinition.h"
#include "MWeaponItemDefinition.generated.h"

class UGameplayEffect;
class UAnimMontage;

UCLASS()
class PROJECTM_API UMWeaponItemDefinition : public UMEquipmentItemDefinition
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMWeaponItemDefinition();

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DefaultAttackEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DefaultAttackMontage;
};