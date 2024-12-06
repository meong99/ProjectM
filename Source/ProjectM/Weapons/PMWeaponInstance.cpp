#include "PMWeaponInstance.h"
#include "../Cosmetics/PMCosmeticAnimationTypes.h"
#include "Animation/AnimInstance.h"

UPMWeaponInstance::UPMWeaponInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

TSubclassOf<UAnimInstance> UPMWeaponInstance::PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const
{
	const FPMAnimInstanceSelectionSet& setToQuery = bEquipped ? EquippedAnimSet : UnEquippedAnimSet;

	return setToQuery.SelectBestAnimInstance(CosmeticTags);
}
