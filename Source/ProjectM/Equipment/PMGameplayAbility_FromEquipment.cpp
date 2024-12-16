// Fill out your copyright notice in the Description page of Project Settings.


#include "PMGameplayAbility_FromEquipment.h"
#include "GameplayAbilitySpec.h"
#include "PMEquipmentInstance.h"
#include "Inventory/PMInventoryItemInstance.h"

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

UPMInventoryItemInstance* UPMGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UPMEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<UPMInventoryItemInstance>(Equipment->GetInstigator());
	}

	return nullptr;
}
