#include "PMCosmeticAnimationTypes.h"

TSubclassOf<UAnimInstance> FPMAnimInstanceSelectionSet::SelectBestAnimInstance(const FGameplayTagContainer& CosmeticTags) const
{
	for (const FPMAnimInstanceSelectionEntry& Rule : AnimInstanceRules)
	{
		if (Rule.AnimInstance != nullptr && CosmeticTags.HasAll(Rule.RequireTags))
		{
			return Rule.AnimInstance;
		}
	}

	return DefaultAnimInstance;
}

USkeletalMesh* FPMBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const
{
	for (const FPMBodyStyleSelectionEntry& Rule : MeshRules)
	{
		if (Rule.Mesh && CosmeticTags.HasAll(CosmeticTags))
		{
			return Rule.Mesh;
		}
	}

	return DefaultMesh;
}