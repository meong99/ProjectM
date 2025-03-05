#pragma once

#include "CoreMinimal.h"
#include "Inventory/PMInventoryItemDefinition.h"
#include "Equipment/PMEquipmentDefinition.h"
#include "PMInventoryFragment_EquippableItem.generated.h"

UCLASS()
class PROJECTM_API UPMInventoryFragment_EquippableItem : public UPMInventoryItemFragment
{
	GENERATED_BODY()
/*
* Overrided Functions
*/
public:

/*
* Member Functions
*/
public:
	TSubclassOf<UPMEquipmentDefinition> GetEquipmentDefinition() const { return EquipmentDefinition; }

/*
* Member Variables
*/
private:
	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<UPMEquipmentDefinition> EquipmentDefinition;
};