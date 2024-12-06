#include "PMInventoryItemInstance.h"
#include "PMInventoryItemDefinition.h"

UPMInventoryItemInstance::UPMInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
{
}

const UPMInventoryItemFragment* UPMInventoryItemInstance::FindFragmentByClass(TSubclassOf<UPMInventoryItemFragment> FragmentClass) const
{
	if (ItemDef != nullptr && FragmentClass != nullptr)
	{
		return GetDefault<UPMInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}
