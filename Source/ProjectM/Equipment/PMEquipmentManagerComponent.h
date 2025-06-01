#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/PMAbilitySet.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Components/MControllerComponentBase.h"
#include "MEquipmentItemList.h"
#include "Types/MItemTypes.h"

#include "PMEquipmentManagerComponent.generated.h"

class UMEquipmentItemDefinition;
class UMEquipmentItemInstance;
class UPMAbilitySystemComponent;
class UPMAbilitySet;

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
	virtual void OnServerRestartPlayer() override;
	virtual void BeginDestroy() override;
	virtual void OnPossess(APawn* aPawn) override;

/*
* Member Functions
*/
public:
	void EquipItem(const int32 ItemRowId);
	void UnequipItem(EMEquipmentItemType EquipmentItemType);

protected:
	UMEquipmentItemInstance*	FindEquippedItemInstance(EMEquipmentItemType EquipmentItemType);
	FMEquipmentItemEntry*		FindEntry(EMEquipmentItemType EquipmentItemType);

	void EquipItem_Impl(const UMEquipmentItemDefinition* EquipDef);
	void RemoveEntry(EMEquipmentItemType EquipmentItemType);

	void GiveAbilities(const UMEquipmentItemDefinition* EquipDef, UMEquipmentItemInstance* Instance);
	void TakeAbilitiesFromAsc(UMEquipmentItemInstance* Instance);

	void EquipAllItems();
	void UnequipAllItems();
	void EquipDefaultWeapon();

	UPMAbilitySystemComponent* GetAbilitySystemComponent() const;
/*
* Member Variables
*/
private:
	UPROPERTY(Replicated)
	FMEquipmentItemList EquippedItemList;

	UPROPERTY()
	TMap<int32, FMAbilitySet_GrantedHandles> GrantedHandles;

	UPROPERTY()
	TMap<EMEquipmentItemType, FMItemHandle> EquippedEntryMap;

	bool bIsInitialized = false;
};
