// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PMGameplayAbilitiy.h"
#include "PMGameplayAbility_FromEquipment.generated.h"

class UPMEquipmentInstance;

/**
 * 
 */
UCLASS()
class PROJECTM_API UPMGameplayAbility_FromEquipment : public UPMGameplayAbilitiy
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
	UPMEquipmentInstance* GetAssociatedEquipment() const;

/*
* Member Variables
*/
private:
};
