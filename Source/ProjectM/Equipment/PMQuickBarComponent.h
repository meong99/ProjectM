#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "PMQuickBarComponent.generated.h"

class UPMInventoryItemInstance;
class UPMEquipmentInstance;
class UPMEquipmentManagerComponent;

/**
 *
 */
UCLASS()
class PROJECTM_API UPMQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

/*
* Member Functions
*/
public:
// 	UFUNCTION(BlueprintCallable)
// 	void AddItemToSlot(const int32 SlotIndex, UPMInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable)
	void SetActiveSlotIndex(const int32 NewIndex);

	UPMEquipmentManagerComponent* FindEquipmentManager() const;
	void UnequipItemInSlot();
	void EquipItemInSlot();

/*
* Member Variables
*/
private:
	UPROPERTY()
	int32 NumSlots = 3;

// 	UPROPERTY()
// 	TArray<TObjectPtr<UPMInventoryItemInstance>> Slots;

	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	UPROPERTY()
	TObjectPtr<UPMEquipmentInstance> EquippedItem;
};