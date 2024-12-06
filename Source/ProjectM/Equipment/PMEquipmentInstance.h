#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PMEquipmentInstance.generated.h"

struct FPMEquipmentActorToSpawn;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTM_API UPMEquipmentInstance : public UObject
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

/*
* Member Functions
*/
public:
	UFUNCTION(BlueprintPure, Category = "Equipment")
	APawn*	GetPawn() const;
	
	UFUNCTION(BlueprintPure, Category = "Equipment", meta = (DeterminesOutputType = PawnType))
	APawn*	GetTypedPawn(TSubclassOf<APawn> PawnType) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName = "OnEquipped"))
	void	K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName = "OnUnequipped"))
	void	K2_OnUnequipped();

	void	SpawnEquipmentActors(const TArray<FPMEquipmentActorToSpawn>& ActorsToSpawn);
	void	DestroyEquipmentActors();

	virtual void	OnEquipped();
	virtual void	OnUnequipped();

	UObject*							GetInstigator() const { return Instigator; }
	const TArray<TObjectPtr<AActor>>&	GetSpawnedActors() const { return SpawnedActors; }

	void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }
/*
* Member Variables
*/
private:
	// 어떤 InventoryItemInstance에 의해 활성화되었는가
	UPROPERTY()
	TObjectPtr<UObject> Instigator;

	// EquipmentDefinition에 따라서 Instancing된 Actor들
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};