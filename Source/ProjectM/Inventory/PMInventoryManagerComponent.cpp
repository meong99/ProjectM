#include "PMInventoryManagerComponent.h"
#include "PMInventoryItemInstance.h"
#include "PMInventoryItemDefinition.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

/*
* UPMInventoryManagerComponent -------------------------------
*/
UPMInventoryManagerComponent::UPMInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UPMInventoryManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

// 	InitInventory();
}

void UPMInventoryManagerComponent::CallOrRegister_OnInitInventory(FOnInitInventory::FDelegate&& Delegate)
{
	if (bIsInitInventory)
	{
		Delegate.Execute(InventoryList);
	}
	else
	{
		Delegate_OnInitInventory.Add(MoveTemp(Delegate));
	}
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

FMItemHandle UPMInventoryManagerComponent::AddItemDefinition(TSubclassOf<UPMInventoryItemDefinition> ItemDef)
{
	FMItemHandle Handle = FMItemHandle{};

	if (ItemDef)
	{
		Handle = InventoryList.AddEntry(ItemDef);

		FPMInventoryEntry* Entry = InventoryList.FindEntry(Handle);
		if (Entry == nullptr)
		{
			return FMItemHandle{};
		}

		UPMInventoryItemInstance* ItemInstance = Entry->Instance;

		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
		{
			AddReplicatedSubObject(ItemInstance);
		}

		Delegate_OnNewItemAdded.Broadcast(Entry);
	}

	#pragma TODO("테스트 완료 후 제거")
	InitInventory();
	return Handle;
}

UPMInventoryItemInstance* UPMInventoryManagerComponent::GetItemInstance(const FMItemHandle& ItemHandle)
{
	FPMInventoryEntry* Entry = InventoryList.FindEntry(ItemHandle);
	if (Entry)
	{
		return Entry->Instance;
	}

	return nullptr;
}

int32 UPMInventoryManagerComponent::ChangeItemQuantity(const FMItemHandle& ItemHandle, int32 ChangeNum)
{
	int32 CurrentItemQuentity = InventoryList.ChangeItemQuantity(ItemHandle, ChangeNum);

	if (CurrentItemQuentity == 0)
	{
		RemoveItem(ItemHandle);
	}

	FOnChangeInventory* InventoryDelegates = Delegate_OnChangeInventory.Find(ItemHandle.ItemUid);
	if (InventoryDelegates)
	{
		InventoryDelegates->Broadcast(ItemHandle);
	}

	return CurrentItemQuentity;
}

FDelegateHandle UPMInventoryManagerComponent::AddDelegateOnChangeInventory(const int32 ItemUid, FOnChangeInventory::FDelegate&& Delegate)
{
	FOnChangeInventory& InventoryDelegates = Delegate_OnChangeInventory.FindOrAdd(ItemUid);

	FDelegateHandle DelegateHandle = InventoryDelegates.Add(Delegate);

	return DelegateHandle;
}

void UPMInventoryManagerComponent::RemoveDelegateOnChangeInventory(const int32 ItemUid, const FDelegateHandle& DelegateHandle)
{
	FOnChangeInventory* InventoryDelegates = Delegate_OnChangeInventory.Find(ItemUid);

	if (InventoryDelegates)
	{
		InventoryDelegates->Remove(DelegateHandle);
		
		if (InventoryDelegates->IsBound() == false)
		{
			Delegate_OnChangeInventory.Remove(ItemUid);
		}
	}
}

void UPMInventoryManagerComponent::InitInventory()
{
	if (bIsInitInventory)
	{
		return;
	}
	bIsInitInventory = true;
	Delegate_OnInitInventory.Broadcast(InventoryList);
	Delegate_OnInitInventory.Clear();
}

void UPMInventoryManagerComponent::RemoveItem(const FMItemHandle& ItemHandle)
{
	FPMInventoryEntry* Entry = InventoryList.FindEntry(ItemHandle);
	if (Entry == nullptr)
	{
		return;
	}

	if (Entry->Instance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(Entry->Instance);
	}

	InventoryList.RemoveEntry(ItemHandle);
}
