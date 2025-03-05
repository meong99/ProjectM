#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "Templates/SubclassOf.h"

#include "PMQuickBarComponent.generated.h"

class UPMInventoryItemInstance;
class UMEquipmentItemInstance;
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
	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(const int32 SlotIndex, UPMInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable)
	void	SetActiveSlotIndex(const int32 NewIndex);
	int32	GetActiveSlotIndex() const { return ActiveSlotIndex; }

	UPMEquipmentManagerComponent* FindEquipmentManager() const;
	void UnequipItemInSlot();
	void EquipItemInSlot();

	const UPMInventoryItemInstance* GetEquippedItemDef() const;

protected:
	UFUNCTION()
	void OnRep_Slots();

	UFUNCTION()
	void OnRep_ActiveSlotIndex();
/*
* Member Variables
*/
private:
	UPROPERTY()
	int32 NumSlots = 3;

	UPROPERTY(ReplicatedUsing = OnRep_Slots)
	TArray<TObjectPtr<UPMInventoryItemInstance>> Slots;

	// 현재 슬롯에서 장착중인 Index. 
	UPROPERTY(ReplicatedUsing = OnRep_ActiveSlotIndex)
	int32 ActiveSlotIndex = INDEX_NONE;

	UPROPERTY()
	TObjectPtr<UMEquipmentItemInstance> EquippedItem;
};