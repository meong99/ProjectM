#include "PMInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"

UPMInventoryItemDefinition::UPMInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
{
}

const UPMInventoryItemFragment* UPMInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UPMInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		for (UPMInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}
