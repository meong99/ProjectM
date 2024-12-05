#include "PMCosmeticAnimationTypes.h"

TSubclassOf<UAnimInstance> FPMAnimLayerSelectionSet::SelectBestLayer(const FGameplayTagContainer& CosmeticTags) const
{
	for (const FPMAnimLayerSelectionEntry& Rule : LayerRules)
	{
		if (Rule.Layer != nullptr && CosmeticTags.HasAll(Rule.RequireTags))
		{
			return Rule.Layer;
		}
	}

	return DefaultLayer;
}

USkeletalMesh* FPMAnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const
{
	for (const FPMAnimBodyStyleSelectionEntry& Rule : MeshRules)
	{
		if (Rule.Mesh && CosmeticTags.HasAll(CosmeticTags))
		{
			return Rule.Mesh;
		}
	}

	return DefaultMesh;
}