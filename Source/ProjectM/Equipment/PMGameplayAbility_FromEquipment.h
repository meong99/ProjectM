// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PMGameplayAbility.h"
#include "PMGameplayAbility_FromEquipment.generated.h"

class UMEquipmentItemInstance;
class UPMInventoryItemInstance;

/**
 * 
 */
UCLASS()
class PROJECTM_API UPMGameplayAbility_FromEquipment : public UPMGameplayAbility
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMGameplayAbility_FromEquipment();

/*
* Member Functions
*/
public:
	UMEquipmentItemInstance* GetAssociatedEquipment() const;
	UPMInventoryItemInstance* GetAssociatedItem() const;

/*
* Member Variables
*/
private:
};
