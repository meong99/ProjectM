#include "Inventory/MInventoryTypes.h"
#include "Engine/Engine.h"
#include "Components/ActorComponent.h"
#include "PMInventoryItemDefinition.h"
#include "PMInventoryItemInstance.h"
#include "PMGameplayTags.h"

/*
* FPMInventoryList -------------------------------
*/
FPMInventoryList::FPMInventoryList()
{
	if (GEngine && GEngine->GetWorld() && GEngine->GetWorld()->GetGameInstance()
			&& IsValid(OwnerComponent) == false)
	{
		MCHAE_FETAL("OwnerComponent must be set!!");
	}
}

FPMInventoryList::FPMInventoryList(UActorComponent* InOwnerComponent)
	: OwnerComponent(InOwnerComponent)
{
	if (IsValid(OwnerComponent) == false)
	{
		MCHAE_FETAL("OwnerComponent must be set!!");
	}
}

void FPMInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
// 	for (int32 Index : RemovedIndices)
// 	{
// 		FPMInventoryEntry& Stack = Entries[Index];
// 		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
// 		Stack.LastObservedCount = 0;
// 	}
}

void FPMInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
// 	for (int32 Index : AddedIndices)
// 	{
// 		FPMInventoryEntry& Stack = Entries[Index];
// 		BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ Stack.StackCount);
// 		Stack.LastObservedCount = Stack.StackCount;
// 	}
}

void FPMInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
// 	for (int32 Index : ChangedIndices)
// 	{
// 		FPMInventoryEntry& Stack = Entries[Index];
// 		check(Stack.LastObservedCount != INDEX_NONE);
// 		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ Stack.StackCount);
// 		Stack.LastObservedCount = Stack.StackCount;
// 	}
}

int32 FPMInventoryList::ChangeItemQuantity(const FMItemHandle& ItemHandle, int32 ChangeNum)
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

FMItemHandle FPMInventoryList::AddEntry(TSubclassOf<UPMInventoryItemDefinition> ItemDef)
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

	FPMInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UPMInventoryItemInstance>(OwningActor);
	NewEntry.Instance->ItemDef = ItemDef;

	for (const UPMInventoryItemFragment* Fragment : GetDefault<UPMInventoryItemDefinition>(ItemDef)->GetFragments())
	{
		if (Fragment)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}

	UPMInventoryItemInstance* ItenInstance = NewEntry.Instance;

#pragma TODO("이거 static쓰지말고 해싱값을 쓰던 뭘 하던 해야함")
	static int32 TempItemUid = 0;
	NewEntry.ItemUid = TempItemUid;
	NewHandle.ItemUid = TempItemUid;

	MarkItemDirty(NewEntry);

	return NewHandle;
}

void FPMInventoryList::RemoveEntry(const FMItemHandle& ItemHandle)
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

FPMInventoryEntry* FPMInventoryList::FindEntry(const FMItemHandle& ItemHandle)
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
