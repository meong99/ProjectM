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

void FPMInventoryItemList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	 	for (int32 Index : RemovedIndices)
	 	{
	 		FPMInventoryEntry& Stack = Entries[Index];
	 	}
}

void FPMInventoryItemList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		if (Entries.IsValidIndex(Index))
		{
	 		OwnerComponent->Delegate_OnNewItemAdded.Broadcast(Entries[Index]);
		}
	}
}

void FPMInventoryItemList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		if (Entries.IsValidIndex(Index))
		{
			FPMInventoryEntry& Entry = Entries[Index];
			FOnChangeInventory Delegate = OwnerComponent->Delegate_OnChangeInventory.FindRef(Entry.ItemUid);

			FMItemHandle Handle;

			Handle.ItemUid = Entry.ItemUid;
			Delegate.Broadcast(Handle);
		}
	}
}

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
		return Entry->Instance->AddStatTagStack(FPMGameplayTags::Get().Item_Quentity, ChangeNum);
	}
	else if (ChangeNum < 0)
	{
		return Entry->Instance->RemoveStatTagStack(FPMGameplayTags::Get().Item_Quentity, ChangeNum);
	}

	MCHAE_WARNING("StackCount is zero");
	return -1;
}

FMItemHandle FPMInventoryItemList::AddEntry(TSubclassOf<UPMInventoryItemDefinition> ItemDef)
{
	FMItemHandle NewHandle = FMItemHandle{};
	if (!ItemDef || !OwnerComponent)
	{
		MCHAE_WARNING("ItemDefinition is not valid");
		return NewHandle;
	}

	AActor* OwningActor = OwnerComponent->GetOwner();
	if (!OwningActor->HasAuthority())
	{
		MCHAE_WARNING("AddEntry only allowed on authority. You called on client");
		return NewHandle;
	}

	const UPMInventoryItemDefinition* ItemDefCDO = GetDefault<UPMInventoryItemDefinition>(ItemDef);
	TSubclassOf<UPMInventoryItemInstance> InstanceType = ItemDefCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = UPMInventoryItemInstance::StaticClass();
	}

	FPMInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UPMInventoryItemInstance>(OwningActor, InstanceType);
	NewEntry.Instance->ItemDef = ItemDef;

	for (const UPMInventoryItemFragment* Fragment : GetDefault<UPMInventoryItemDefinition>(ItemDef)->GetFragments())
	{
		if (Fragment)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}

	UPMInventoryItemInstance* ItemInstance = NewEntry.Instance;

#pragma TODO("이거 static쓰지말고 해싱값을 쓰던 뭘 하던 해야함")
	static int32 TempItemUid = 0;
	NewEntry.ItemUid = TempItemUid;
	NewHandle.ItemUid = TempItemUid;
	NewHandle.ItemType = ItemInstance->GetItemType();
	ItemInstance->ItemHandle = NewHandle;
	TempItemUid++;
	MarkItemDirty(NewEntry);

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
