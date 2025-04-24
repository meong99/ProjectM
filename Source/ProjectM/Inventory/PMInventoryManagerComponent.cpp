#include "PMInventoryManagerComponent.h"
#include "PMInventoryItemInstance.h"
#include "PMInventoryItemDefinition.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "System/MDataTableManager.h"
#include "Engine/Engine.h"
#include "Engine/DataTable.h"
#include "Table/Item/MTable_ConsumableItem.h"
#include "Table/MTableAsset.h"
#include "Table/Item/MTable_EquipmentItem.h"

/*
* UPMInventoryManagerComponent -------------------------------
*/
UPMInventoryManagerComponent::UPMInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
	, ConsumableItemList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;

	InventoryList.OwnedItemType = EMItemType::Equipment;
	ConsumableItemList.OwnedItemType = EMItemType::Consumable;
}

void UPMInventoryManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	InitInventory();
}

void UPMInventoryManagerComponent::CallOrRegister_OnInitInventory(FOnInitInventory::FDelegate&& Delegate)
{
	if (bIsInitInventory)
	{
		Delegate.Execute(InventoryList);
		Delegate.Execute(ConsumableItemList);
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
	DOREPLIFETIME(ThisClass, ConsumableItemList);
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

	for (FPMInventoryEntry& Entry : ConsumableItemList.Entries)
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

		for (const FPMInventoryEntry& Entry : ConsumableItemList.Entries)
		{
			UPMInventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

FMItemHandle UPMInventoryManagerComponent::AddItemDefinition(int32 ItemRowId)
{
	UMDataTableManager* TableManager = GEngine->GetEngineSubsystem<UMDataTableManager>();
	if (TableManager)
	{
		return AddItemDefinition(TableManager->GetDefinition<UPMInventoryItemDefinition>(ItemRowId));
	}

	return {};
}

FMItemHandle UPMInventoryManagerComponent::AddItemDefinition(TSubclassOf<UPMInventoryItemDefinition> ItemDef)
{
	FMItemHandle Handle = FMItemHandle{};
	UPMInventoryItemDefinition* CDO = ItemDef->GetDefaultObject<UPMInventoryItemDefinition>();
	if (CDO)
	{
		FPMInventoryItemList* ItemList = GetItemList(CDO->ItemType);
		if (ItemList)
		{
			FPMInventoryEntry* Entry = ItemList->FindEntry(ItemDef);
			if (Entry && ItemList->OwnedItemType != EMItemType::Equipment)
			{
				ItemList->ChangeItemQuantity(Handle, 1);
				return Entry->GetItemHandle();
			}
			else
			{
				return AddItemDefinition_Impl(ItemDef, *ItemList);
			}
		}
	}

	return Handle;
}

FMItemHandle UPMInventoryManagerComponent::AddItemDefinition_Impl(TSubclassOf<UPMInventoryItemDefinition> ItemDef, FPMInventoryItemList& ItemList)
{
	FMItemHandle Handle = FMItemHandle{};

	Handle = ItemList.AddEntry(ItemDef);

	FPMInventoryEntry* Entry = ItemList.FindEntry(Handle);
	if (Entry == nullptr)
	{
		return FMItemHandle{};
	}

	UPMInventoryItemInstance* ItemInstance = Entry->Instance;

	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}
	Delegate_NotifyItemAdded.Broadcast(*Entry);
	Delegate_OnNewItemAdded.Broadcast(*Entry);

	return Handle;
}

void UPMInventoryManagerComponent::Multicast_OnRemoveItem_Implementation(const FMItemHandle& ItemHandle, const EMItemType ItemType)
{
	Delegate_OnRemoveItem.Broadcast(ItemHandle, ItemType);
}

UPMInventoryItemInstance* UPMInventoryManagerComponent::FindItemInstance(const FMItemHandle& ItemHandle)
{
	FPMInventoryItemList* ItemList = GetItemList(ItemHandle.ItemType);
	if (ItemList)
	{
		FPMInventoryEntry* Entry = ItemList->FindEntry(ItemHandle);
		if (Entry)
		{
			return Entry->Instance;
		}
	}

	return nullptr;
}

FPMInventoryEntry* UPMInventoryManagerComponent::FindEntry(const FMItemHandle& ItemHandle)
{
	FPMInventoryItemList* ItemList = GetItemList(ItemHandle.ItemType);
	if (ItemList)
	{
		return ItemList->FindEntry(ItemHandle);
	}

	return nullptr;
}

FPMInventoryEntry* UPMInventoryManagerComponent::FindEntry(TSubclassOf<UPMInventoryItemDefinition> ItemDef)
{
	UPMInventoryItemDefinition* CDO = ItemDef->GetDefaultObject<UPMInventoryItemDefinition>();
	if (CDO)
	{
		FPMInventoryItemList* ItemList = GetItemList(CDO->ItemType);
		if (ItemList)
		{
			return ItemList->FindEntry(ItemDef);
		}
	}

	return nullptr;
}

int32 UPMInventoryManagerComponent::ChangeItemQuantity(const FMItemHandle& ItemHandle, int32 ChangeNum)
{
	FPMInventoryItemList* ItemList = GetItemList(ItemHandle.ItemType);
	if (ItemList)
	{
		int32 CurrentItemQuentity = ItemList->ChangeItemQuantity(ItemHandle, ChangeNum);

		if (CurrentItemQuentity == 0)
		{
			RemoveItem(ItemHandle);
		}

		if (HasAuthority())
		{
			FOnChangeInventory* InventoryDelegates = Delegate_OnChangeInventory.Find(ItemHandle.ItemUid);
			if (InventoryDelegates)
			{
				InventoryDelegates->Broadcast(ItemHandle);
			}
		}

		return CurrentItemQuentity;
	}

	return 0;
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

void UPMInventoryManagerComponent::Server_UseItem_Implementation(const FMItemHandle& ItemHandle)
{
	UPMInventoryItemInstance* ItemInstance = FindItemInstance(ItemHandle);
	if (ItemInstance)
	{
		if (ItemInstance->UseItem() == 0)
		{
			RemoveItem(ItemHandle);
		}
	}
}

void UPMInventoryManagerComponent::InitInventory()
{
	bIsInitInventory = true;
	Delegate_OnInitInventory.Broadcast(InventoryList);
	Delegate_OnInitInventory.Broadcast(ConsumableItemList);
	Delegate_OnInitInventory.Clear();
}

void UPMInventoryManagerComponent::RemoveItem(const FMItemHandle& ItemHandle)
{
	FPMInventoryItemList* ItemList = GetItemList(ItemHandle.ItemType);
	if (ItemList)
	{
		FPMInventoryEntry* Entry = ItemList->FindEntry(ItemHandle);
		if (Entry)
		{
			if (Entry->Instance && IsUsingRegisteredSubObjectList())
			{
				RemoveReplicatedSubObject(Entry->Instance);
			}

			ItemList->RemoveEntry(ItemHandle);
			Multicast_OnRemoveItem(ItemHandle, ItemList->OwnedItemType);
		}
	}
	
}

FPMInventoryItemList* UPMInventoryManagerComponent::GetItemList(const EMItemType ItemType)
{
	switch (ItemType)
	{
		case EMItemType::Equipment :
		{
			return &InventoryList;
		}
		case EMItemType::Consumable :
		{
			return &ConsumableItemList;
		}
		default:
		{
			MCHAE_WARNING("Can't Found ItemList");
			return nullptr;
		}
	}
}

void UPMInventoryManagerComponent::Debug_AddItem(int32 RowId)
{
	if (GEngine)
	{
		UMDataTableManager* TableManager = GEngine->GetEngineSubsystem<UMDataTableManager>();
		if (TableManager)
		{
			const UDataTable* DataTable = TableManager->GetDataTable(RowId);
			if (DataTable)
			{
				int32 ElementId = UMDataTableManager::ChangeRowIdToElementId(RowId);
				const TArray<FName>& Names = DataTable->GetRowNames();
				if (Names.IsValidIndex(ElementId))
				{
					FMTable_TableBase* Item = DataTable->FindRow<FMTable_TableBase>(Names[ElementId], Names[ElementId].ToString());
					if (Item)
					{
						DebugServer_AddItem(Item->GetDefinition<UPMInventoryItemDefinition>());
					}
					else
					{
						MCHAE_LOG("Can't Found Item. RowId = %d", RowId);
					}
				}
			}
		}
	}
}

void UPMInventoryManagerComponent::DebugServer_AddItem_Implementation(TSubclassOf<UPMInventoryItemDefinition> ItemDef)
{
	AddItemDefinition(ItemDef);
}
