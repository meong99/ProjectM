#include "PMInventoryItemInstance.h"
#include "PMInventoryItemDefinition.h"
#include "Net/UnrealNetwork.h"
#include "PMGameplayTags.h"
#include "PMInventoryManagerComponent.h"

UPMInventoryItemInstance::UPMInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
{
}

void UPMInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, StatTags);
	DOREPLIFETIME(ThisClass, ItemDef);
	DOREPLIFETIME(ThisClass, ItemRowId);
	DOREPLIFETIME(ThisClass, ItemHandle);
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

int32 UPMInventoryItemInstance::UseItem()
{
	AActor* Outer = Cast<AActor>(GetOuter());
	UPMInventoryManagerComponent* InventoryComp = Outer ? Outer->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
	if (InventoryComp && CanUseItem())
	{
		return InventoryComp->ChangeItemQuantity(ItemHandle, -1);
	}

	return GetStatTagStackCount(FPMGameplayTags::Get().Item_Quentity);
}

int32 UPMInventoryItemInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	int32 Count = StatTags.AddStack(Tag, StackCount);

	return Count;
}

int32 UPMInventoryItemInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	int32 Count = StatTags.RemoveStack(Tag, StackCount);

	return Count;
}

bool UPMInventoryItemInstance::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

int32 UPMInventoryItemInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

EMItemType UPMInventoryItemInstance::GetItemType() const
{
	if (ItemDef)
	{
		UPMInventoryItemDefinition* DefinitionCDO = ItemDef->GetDefaultObject<UPMInventoryItemDefinition>();
		return DefinitionCDO->ItemType;
	}

	MCHAE_WARNING("ItemDefinition is not defined. Please set item definition ");

	return EMItemType::None;
}
