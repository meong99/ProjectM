#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include <Components\ActorComponent.h>

#include "PMEquipmentManagerComponent.generated.h"

class UPMEquipmentDefinition;
class UPMEquipmentInstance;

USTRUCT(BlueprintType)
struct FPMAppliedEquipmentEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<UPMEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UPMEquipmentInstance> Instance = nullptr;
};

USTRUCT(BlueprintType)
struct FPMEquipmentList
{
	GENERATED_BODY()

	FPMEquipmentList();
	FPMEquipmentList(UActorComponent* InOwnerComponent);

	UPMEquipmentInstance* AddEntry(TSubclassOf<UPMEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UPMEquipmentInstance* Instance);

	UPROPERTY()
	TArray<FPMAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

/**
 *
 */
UCLASS()
class PROJECTM_API UPMEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

/*
* Member Functions
*/
public:
	UPMEquipmentInstance* EquipItem(TSubclassOf<UPMEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(UPMEquipmentInstance* ItemInstance);

	const FPMEquipmentList& GetEquipmentList() const { return EquipmentList; }

/*
* Member Variables
*/
private:
	UPROPERTY()
	FPMEquipmentList EquipmentList;
};