#pragma once

#include "CoreMinimal.h"
#include "Inventory/PMInventoryItemInstance.h"
#include "GameFramework/Pawn.h"
#include "MEquipmentItemDefinition.h"
#include "MEquipmentItemInstance.generated.h"

class UPMAbilitySystemComponent;
class UAnimMontage;

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTM_API UMEquipmentItemInstance : public UPMInventoryItemInstance
{
	GENERATED_BODY()

	/*
	* Overrided Functions
	*/
public:
	UMEquipmentItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual int32 UseItem() override;
	virtual bool CanUseItem() const override;

/*
* Member Functions
*/
public:
	virtual void	OnEquipped();
	virtual void	OnUnequipped();

	void	SpawnEquipmentActors(const TArray<FPMEquipmentActorToSpawn>& ActorsToSpawn);
	void	DestroyEquipmentActors();

	UFUNCTION(BlueprintPure, Category = "Equipment")
	UObject*				GetInstigator() { return this; }
	UFUNCTION(BlueprintPure, Category = "Equipment")
	APawn*					GetPawn() const;
	// DeterminesOutputType을 지정해주면 BP에서 내가 원하는 Type으로 형변환해서 돌려준다
	UFUNCTION(BlueprintPure, Category = "Equipment", meta = (DeterminesOutputType = PawnType))
	APawn*					GetTypedPawn(TSubclassOf<APawn> PawnType) const;
	UFUNCTION(BlueprintPure, Category = "Equipment")
	const TArray<AActor*>&	GetSpawnedActors() const { return SpawnedActors; }

protected:
	UPMAbilitySystemComponent* GetAbilitySystemComponent() const;
/*
* Member Variables
*/
protected:
	// EquipmentDefinition에 따라서 Instancing된 Actor들
	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;
};