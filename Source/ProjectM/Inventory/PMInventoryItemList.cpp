#include "Inventory/PMInventoryItemList.h"
#include "Engine/Engine.h"
#include "Components/ActorComponent.h"
#include "PMInventoryItemDefinition.h"
#include "PMInventoryItemInstance.h"
#include "PMGameplayTags.h"
#include "PMInventoryManagerComponent.h"

/*
* FPMInventoryEntry -------------------------------
*/
bool FPMInventoryEntry::IsValid() const
{
	return ItemUid != INDEX_NONE && Instance != nullptr;
}

EMItemType FPMInventoryEntry::GetItemType() const
{
	if (Instance)
	{
		return Instance->GetItemType();
	}

	MCHAE_WARNING("Item instance is not valid.");

	return EMItemType::None;
}

FMItemHandle FPMInventoryEntry::GetItemHandle() const
{
	if (Instance)
	{
		return Instance->ItemHandle;
	}

	MCHAE_WARNING("Item instance is not valid.");

	return FMItemHandle{};
}

int32 FPMInventoryEntry::GetItemRowId() const
{
	if (Instance)
	{
		return Instance->ItemRowId;
	}

	return 0;
}

TSubclassOf<UPMInventoryItemDefinition> FPMInventoryEntry::GetItemDefinition() const
{
	if (Instance)
	{
		return Instance->ItemDef;
	}

	return nullptr;
}

/*
* FPMInventoryList -------------------------------
*/
FPMInventoryItemList::FPMInventoryItemList()
{
	if (GEngine && GEngine->GetWorld() && GEngine->GetWorld()->GetGameInstance()
		&& IsValid(OwnerComponent) == false)
	{
		MCHAE_FETAL("OwnerComponent must be set!!");
	}
}

FPMInventoryItemList::FPMInventoryItemList(UPMInventoryManagerComponent* InOwnerComponent)
	: OwnerComponent(InOwnerComponent)
{
	if (IsValid(OwnerComponent) == false)
	{
		MCHAE_FETAL("OwnerComponent must be set!!");
	}
}
// 
// void FPMInventoryItemList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
// {
// }
// 
// void FPMInventoryItemList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
// {
// }
// 
// void FPMInventoryItemList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
// {
// }

int32 FPMInventoryItemList::ChangeItemQuantity(const FMItemHandle& ItemHandle, int32 ChangeNum)
{
	FPMInventoryEntry* Entry = FindEntry(ItemHandle);

	if (Entry == nullptr)
	{
		MCHAE_WARNING("Can't found item.");
		return -1;
	}

	if (Entry->Instance == nullptr)
	{
		MCHAE_WARNING("It's invalid Entry! Item intance is not created or removed in entry.");
		return -1;
	}

	if (ChangeNum > 0)
	{
		MarkItemDirty(*Entry);
		return Entry->Instance->AddStatTagStack(FPMGameplayTags::Get().Item_Quentity, ChangeNum);
	}
	else if (ChangeNum < 0)
	{
		MarkItemDirty(*Entry);
		return Entry->Instance->RemoveStatTagStack(FPMGameplayTags::Get().Item_Quentity, ChangeNum * -1);
	}

	MCHAE_WARNING("StackCount is zero");
	return -1;
}

FMItemHandle FPMInventoryItemList::AddEntry(TSubclassOf<UPMInventoryItemDefinition> ItemDef, int32 ItemQuentity)
{
	if (!ItemDef || !OwnerComponent)
	{
		MCHAE_WARNING("ItemDefinition is not valid");
		return {};
	}

	AActor* OwningActor = OwnerComponent->GetOwner();
	if (!OwningActor->HasAuthority())
	{
		MCHAE_WARNING("AddEntry only allowed on authority. You called on client");
		return {};
	}


	FPMInventoryEntry* NewEntry = MakeEntry(ItemDef, ItemQuentity);
	AddEntry_Impl(*NewEntry);

	return NewEntry->GetItemHandle();
}

FMItemHandle FPMInventoryItemList::AddEntry(UPMInventoryItemInstance* Instance)
{
	if (!Instance)
	{
		MCHAE_WARNING("정상적이지 않은 ItemInstance가 Entry에 추가되려합니다.");
		return {};
	}

	FPMInventoryEntry* NewEntry = MakeEntry(Instance);
	AddEntry_Impl(*NewEntry);

	return NewEntry->GetItemHandle();
}

FMItemHandle FPMInventoryItemList::AddEntry_Impl(FPMInventoryEntry& Entry)
{
	FMItemHandle NewHandle = FMItemHandle{};

	if (!Entry.Instance)
	{
		ensure(false);
		MCHAE_WARNING("ItemInstance is not valid!");

		return NewHandle;
	}

	static int32 ItemUid = 0;

	NewHandle.ItemUid = ItemUid;
	NewHandle.ItemType = Entry.Instance->GetItemType();

	Entry.ItemUid = ItemUid;
	Entry.Instance->ItemHandle = NewHandle;

	ItemUid++;

	MarkItemDirty(Entry);

	return NewHandle;
}

void FPMInventoryItemList::RemoveEntry(const FMItemHandle& ItemHandle)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FPMInventoryEntry& Entry = *EntryIt;
		if (Entry.ItemUid == ItemHandle.ItemUid)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

FPMInventoryEntry* FPMInventoryItemList::MakeEntry(TSubclassOf<UPMInventoryItemDefinition> ItemDef, int32 ItemQuentity)
{
	AActor* OwningActor = OwnerComponent->GetOwner();
	const UPMInventoryItemDefinition* ItemDefCDO = GetDefault<UPMInventoryItemDefinition>(ItemDef);
	TSubclassOf<UPMInventoryItemInstance> InstanceType = ItemDefCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = UPMInventoryItemInstance::StaticClass();
	}

	FPMInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UPMInventoryItemInstance>(OwningActor, InstanceType);
	NewEntry.Instance->ItemDef = ItemDef;
	NewEntry.Instance->ItemRowId = ItemDefCDO->RowId;

	for (const UPMInventoryItemFragment* Fragment : GetDefault<UPMInventoryItemDefinition>(ItemDef)->GetFragments())
	{
		if (Fragment)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}

	NewEntry.Instance->OnInstanceCreated();

	if (ItemQuentity > 1)
	{
		NewEntry.Instance->AddStatTagStack(FPMGameplayTags::Get().Item_Quentity, ItemQuentity - 1);
	}

	return &NewEntry;
}

FPMInventoryEntry* FPMInventoryItemList::MakeEntry(UPMInventoryItemInstance* Instance)
{
	FPMInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = Instance;

	for (const UPMInventoryItemFragment* Fragment : GetDefault<UPMInventoryItemDefinition>(Instance->ItemDef)->GetFragments())
	{
		if (Fragment)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}

	return &NewEntry;
}

FPMInventoryEntry* FPMInventoryItemList::FindEntry(const FMItemHandle& ItemHandle)
{
#pragma TODO("ItemList를 Tree로 구현해서 사용해야한다.")
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FPMInventoryEntry& Entry = *EntryIt;
		if (Entry.ItemUid == ItemHandle.ItemUid)
		{
			return &Entry;
		}
	}

	return nullptr;
}

FPMInventoryEntry* FPMInventoryItemList::FindEntry(TSubclassOf<UPMInventoryItemDefinition> ItemDef)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FPMInventoryEntry& Entry = *EntryIt;
		if (Entry.GetItemDefinition() == ItemDef)
		{
			return &Entry;
		}
	}

	return nullptr;
}
