#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/PMAbilitySet.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Components/MControllerComponentBase.h"

#include "PMEquipmentManagerComponent.generated.h"

class UMEquipmentItemDefinition;
class UMEquipmentItemInstance;
class UPMAbilitySystemComponent;

/**
 *
 */
UCLASS(BlueprintType)
class PROJECTM_API UPMEquipmentManagerComponent : public UMControllerComponentBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginDestroy() override;
	virtual void OnServerRestartPlayer() override;

/*
* Member Functions
*/
public:
	void EquipItem(UMEquipmentItemInstance* ItemInstance);
	void UnequipItem(int32 EquipmentItemType);

protected:
	UFUNCTION(BlueprintCallable)
	UMEquipmentItemInstance* FindEquippedItem(int32 EquipmentItemType);

	UFUNCTION()
	void OnRep_OnChangeEquipedItem();

	void SpawnEquipmentActor();
	void DestroyEquipmentActors();

	UPMAbilitySystemComponent* GetAbilitySystemComponent() const;
/*
* Member Variables
*/
private:
	UPROPERTY(ReplicatedUsing = "OnRep_OnChangeEquipedItem")
	TArray<UMEquipmentItemInstance*> EquippedItems;

	UPROPERTY()
	TMap<int32, FPMAbilitySet_GrantedHandles> GrantedHandles;
};
