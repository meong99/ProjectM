#include "PMInventoryItemInstance.h"
#include "PMInventoryItemDefinition.h"

UPMInventoryItemInstance::UPMInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
{
}

const UPMInventoryItemFragment* UPMInventoryItemInstance::FindFragmentByClass(TSubclassOf<UPMInventoryItemFragment> FragmentClass) const
{
	if (ItemDef != nullptr && FragmentClass != nullptr)
	{
		// Fragment를 DefaultToInstanced로 지정해놨기 때문에 BP에서 값을 변경해도 해당 변경된 값들은 CDO에 저장되어있다.
		return GetDefault<UPMInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	else
	{
		MCHAE_WARNING("ItemDef or FragmentClass is not valid! Check!");
	}

	return nullptr;
}
