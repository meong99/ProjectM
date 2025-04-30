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
#include "PMGameplayTags.h"

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

FMItemHandle UPMInventoryManagerComponent::AddItemtoInventory(int32 ItemRowId)
{
	UMDataTableManager* TableManager = GEngine->GetEngineSubsystem<UMDataTableManager>();
	if (TableManager)
	{
		return AddItemtoInventory(TableManager->GetDefinitionClass<UPMInventoryItemDefinition>(ItemRowId));
	}

	return {};
}

FMItemHandle UPMInventoryManagerComponent::AddItemtoInventory(TSubclassOf<UPMInventoryItemDefinition> ItemDef)
{
	FMItemHandle Handle = FMItemHandle{};
	UPMInventoryItemDefinition* CDO = ItemDef->GetDefaultObject<UPMInventoryItemDefinition>();
	if (!CDO)
	{
		ensure(false);
		MCHAE_WARNING("ItemCDO is not valid");
		return Handle;
	}

	FPMInventoryItemList* ItemList = GetItemList(CDO->ItemType);
	if (ItemList)
	{
		FPMInventoryEntry* Entry = ItemList->FindEntry(ItemDef);
		if (Entry && ItemList->OwnedItemType != EMItemType::Equipment)
		{
			ItemList->ChangeItemQuantity(Entry->GetItemHandle(), 1);
			Broadcast_OnNewItemAdded(*Entry, true);
			return Entry->GetItemHandle();
		}
		else
		{
			return AddItemDefinition_Impl(ItemDef, *ItemList);
		}
	}

	return Handle;
}

FMItemHandle UPMInventoryManagerComponent::ReturnItem(UPMInventoryItemInstance* Instance)
{
	if (Instance)
	{
		FPMInventoryItemList* ItemList = GetItemList(Instance->GetItemType());

		if (ItemList)
		{
			return ItemList->AddEntry(Instance);
		}
	}

	return {};
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

	Broadcast_OnNewItemAdded(*Entry);

	return Handle;
}

void UPMInventoryManagerComponent::Broadcast_OnRemoveItem(const FMItemHandle& ItemHandle, const EMItemType ItemType)
{
	Delegate_OnRemoveItem.Broadcast(ItemHandle, ItemType);
}

void UPMInventoryManagerComponent::Broadcast_OnNewItemAdded(const FPMInventoryEntry& ItemEntry, bool bOnlyNotify)
{
	Delegate_NotifyItemAdded.Broadcast(ItemEntry);
	if (!bOnlyNotify)
	{
		Delegate_OnNewItemAdded.Broadcast(ItemEntry);
	}
}

void UPMInventoryManagerComponent::Broadcast_OnChangeInventory(const FMItemHandle& ItemHandle)
{
	FOnChangeInventory* InventoryDelegates = Delegate_OnChangeInventory.Find(ItemHandle.ItemUid);
	if (InventoryDelegates)
	{
		InventoryDelegates->Broadcast(ItemHandle);
	}
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

int32 UPMInventoryManagerComponent::GetItemQuantity(const int32 ItemRowId)
{
	UMDataTableManager* TableManager = GEngine->GetEngineSubsystem<UMDataTableManager>();
	UPMInventoryItemDefinition* ItemDef = TableManager->GetItemDefinition(ItemRowId);
	if (!ItemDef)
	{
		ensure(false);
		return 0;
	}

	FPMInventoryItemList* ItemList = GetItemList(ItemDef->ItemType);
	if (!ItemList)
	{
		ensure(false);
		return 0;
	}

	int32 Result = 0;
	for (const FPMInventoryEntry& Entry : ItemList->Entries)
	{
		if (Entry.GetItemRowId() == ItemRowId && Entry.Instance)
		{
			Result += Entry.Instance->GetStatTagStackCount(FPMGameplayTags::Get().Item_Quentity);
		}
	}

	return Result;
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

		Broadcast_OnChangeInventory(ItemHandle);
		return CurrentItemQuentity;
	}

	return 0;
}

int32 UPMInventoryManagerComponent::ChangeItemQuantity(int32 ItemRowId, int32 ChangeNum)
{
	UMDataTableManager* TableManager = GEngine->GetEngineSubsystem<UMDataTableManager>();
	UPMInventoryItemDefinition* ItemDef = TableManager->GetItemDefinition(ItemRowId);
	if (!ItemDef)
	{
		ensure(false);
		return 0;
	}

	FPMInventoryItemList* ItemList = GetItemList(ItemDef->ItemType);
	if (!ItemList)
	{
		ensure(false);
		return 0;
	}

	#pragma TODO("이거 아이템 카운트 리팩토링 필요함")
	int32 Result = 0;
	for (auto Iter = ItemList->Entries.CreateIterator(); Iter; ++Iter)
	{
		if (Iter->Instance)
		{
			int32 ItemCount = Iter->Instance->GetStatTagStackCount(FPMGameplayTags::Get().Item_Quentity);
			int32 LeftCount = Iter->Instance->RemoveStatTagStack(FPMGameplayTags::Get().Item_Quentity, ChangeNum);

			if (LeftCount == 0)
			{
				Iter.RemoveCurrent();
			}

			ChangeNum -= (ItemCount - LeftCount);
			Result += LeftCount;
		}
	}

	return Result;
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
			Broadcast_OnRemoveItem(ItemHandle, ItemList->OwnedItemType);
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

#if WITH_EDITOR
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
						DebugServer_AddItem(Item->RowId);
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

void UPMInventoryManagerComponent::DebugServer_AddItem_Implementation(int32 Rowid)
{
	AddItemtoInventory(Rowid);
}
#endif
