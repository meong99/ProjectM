#include "PMInventoryManagerComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "PMInventoryItemInstance.h"
#include "PMInventoryItemDefinition.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

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

void FPMInventoryList::BroadcastChangeMessage(FPMInventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
	FMInventoryChangeMessage Message;
	Message.InventoryOwner = OwnerComponent;
	Message.Instance = Entry.Instance;
	Message.NewCount = NewCount;
	Message.Delta = NewCount - OldCount;

// 	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld());
// 	MessageSystem.BroadcastMessage(TAG_Lyra_Inventory_Message_StackChanged, Message);
}

UPMInventoryItemInstance* FPMInventoryList::AddEntry(TSubclassOf<UPMInventoryItemDefinition> ItemDef)
{
	UPMInventoryItemInstance* Result = nullptr;
	check(ItemDef);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

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

	Result = NewEntry.Instance;
	MarkItemDirty(NewEntry);
	return Result;
}

void FPMInventoryList::AddEntry(UPMInventoryItemInstance* Instance)
{
	unimplemented();
}

void FPMInventoryList::RemoveEntry(UPMInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FPMInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

/*
* UPMInventoryManagerComponent -------------------------------
*/
UPMInventoryManagerComponent::UPMInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

void UPMInventoryManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

bool UPMInventoryManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FPMInventoryEntry& Entry : InventoryList.Entries)
	{
		UPMInventoryItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UPMInventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

		// Register existing ULyraInventoryItemInstance
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FPMInventoryEntry& Entry : InventoryList.Entries)
		{
			UPMInventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

UPMInventoryItemInstance* UPMInventoryManagerComponent::AddItemDefinition(TSubclassOf<UPMInventoryItemDefinition> ItemDef)
{
	UPMInventoryItemInstance* Result = nullptr;
	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);

		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}

	return Result;
}

void UPMInventoryManagerComponent::AddItemInstance(UPMInventoryItemInstance* ItemInstance)
{
	InventoryList.AddEntry(ItemInstance);
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}
}

void UPMInventoryManagerComponent::RemoveItemInstance(UPMInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}
