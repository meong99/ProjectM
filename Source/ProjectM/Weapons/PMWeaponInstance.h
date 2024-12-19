#pragma once

#include "CoreMinimal.h"
#include "Equipment/PMEquipmentInstance.h"
#include "Cosmetics/PMCosmeticAnimationTypes.h"

#include "PMWeaponInstance.generated.h"

class UAnimInstance;

/**
 *
 */
UCLASS()
class PROJECTM_API UPMWeaponInstance : public UPMEquipmentInstance
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

/*
* Member Functions
*/
public:
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
};