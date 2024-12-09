// Fill out your copyright notice in the Description page of Project Settings.


#include "PMGameplayAbility_FromEquipment.h"
#include "GameplayAbilitySpec.h"
#include "PMEquipmentInstance.h"

UPMGameplayAbility_FromEquipment::UPMGameplayAbility_FromEquipment()
{

}

UPMEquipmentInstance* UPMGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UPMEquipmentInstance>(Spec->SourceObject.Get());
	}

	return  nullptr;
}
