#pragma once

#include "CoreMinimal.h"
#include "Inventory/PMInventoryItemInstance.h"
#include "GameFramework/Pawn.h"
#include "MEquipmentItemDefinition.h"
#include "MEquipableItemInstance.generated.h"

class UPMAbilitySystemComponent;
class UAnimMontage;

/**
 *	Inventory에서 아직 장착하지 않은 장비들의 Instance.
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTM_API UMEquipableItemInstance : public UPMInventoryItemInstance
{
	GENERATED_BODY()

	/*
	* Overrided Functions
	*/
public:
	UMEquipableItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool IsSupportedForNetworking() const override { return true; }

	virtual void OnInstanceCreated() override;
	virtual int32 ActivateItem() override;
	virtual bool CanUseItem() const override;

	/*
* Member Functions
*/
public:
	UFUNCTION(BlueprintPure, Category = "Equipment")
	UObject*				GetInstigator() { return this; }

	UFUNCTION(BlueprintPure, Category = "Equipment")
	APawn*					GetPawn() const;

	// DeterminesOutputType을 지정해주면 BP에서 내가 원하는 Type으로 형변환해서 돌려준다
	UFUNCTION(BlueprintPure, Category = "Equipment", meta = (DeterminesOutputType = PawnType))
	APawn*					GetTypedPawn(TSubclassOf<APawn> PawnType) const;

	EMEquipmentItemType		GetEquipmentItemType() const { return EquipmentItemType; }

protected:
	UPMAbilitySystemComponent* GetAbilitySystemComponent() const;

/*
* Member Variables
*/
public:
	UPROPERTY(VisibleAnywhere)
	EMEquipmentItemType EquipmentItemType = EMEquipmentItemType::None;
};