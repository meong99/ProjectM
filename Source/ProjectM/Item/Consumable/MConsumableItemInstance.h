//#pragma once
//
//#include "CoreMinimal.h"
//#include "UObject/Object.h"
//#include "PMEquipmentInstance.generated.h"
//
//struct FPMEquipmentActorToSpawn;
//
///**
// *
// */
//UCLASS(BlueprintType, Blueprintable)
//class PROJECTM_API UPMEquipmentInstance : public UObject
//{
//	GENERATED_BODY()
//
///*
//* Overrided Functions
//*/
//public:
//	UPMEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
//	virtual bool IsSupportedForNetworking() const override { return true; }
//
///*
//* Member Functions
//*/
//public:
//	UFUNCTION(BlueprintPure, Category = "Equipment")
//	APawn*	GetPawn() const;
//	
//	// DeterminesOutputType을 지정해주면 BP에서 내가 원하는 Type으로 형변환해서 돌려준다
//	UFUNCTION(BlueprintPure, Category = "Equipment", meta = (DeterminesOutputType = PawnType))
//	APawn*	GetTypedPawn(TSubclassOf<APawn> PawnType) const;
//
//	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName = "OnEquipped"))
//	void	K2_OnEquipped();
//
//	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName = "OnUnequipped"))
//	void	K2_OnUnequipped();
//
//	void	SpawnEquipmentActors(const TArray<FPMEquipmentActorToSpawn>& ActorsToSpawn);
//	void	DestroyEquipmentActors();
//
//	virtual void	OnEquipped();
//	virtual void	OnUnequipped();
//
//	UFUNCTION(BlueprintPure, Category = "Equipment")
//	UObject*				GetInstigator() const { return Instigator; }
//	UFUNCTION(BlueprintPure, Category = "Equipment")
//	const TArray<AActor*>&	GetSpawnedActors() const { return SpawnedActors; }
//
//	void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }
//
//private:
//	UFUNCTION()
//	void OnRep_Instigator();
///*
//* Member Variables
//*/
//private:
//	// 어떤 InventoryItemInstance에 의해 활성화되었는가
//	UPROPERTY(ReplicatedUsing = OnRep_Instigator)
//	TObjectPtr<UObject> Instigator;
//
//	// EquipmentDefinition에 따라서 Instancing된 Actor들
//	UPROPERTY(Replicated)
//	TArray<TObjectPtr<AActor>> SpawnedActors;
//};