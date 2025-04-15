#pragma once

#include "CoreMinimal.h"
#include "Item/Equipment/MEquipmentItemInstance.h"
#include "Cosmetics/PMCosmeticAnimationTypes.h"
#include "Animation/AnimMontage.h"

#include "PMWeaponInstance.generated.h"

class UAnimInstance;

/**
 *
 */
UCLASS()
class PROJECTM_API UPMWeaponInstance : public UMEquipmentItemInstance
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual int32 UseItem() override;
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;

	/*
* Member Functions
*/
public:
	void DetermineCosmeticTag();
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Animation")
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;

/*
* Member Variables
*/
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Animation")
	FPMAnimInstanceSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Animation")
	FPMAnimInstanceSelectionSet UnEquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Animation")
	UAnimMontage* WeaponMontageToPlay;

	FGameplayTagContainer CosmeticAnimStyle;
};