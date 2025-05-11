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

FMItemHandle UPMInventoryManagerComponent::RequestItemToInventory(const FMItemRequest& ItemRequest)
{
	FMItemHandle Handle = FMItemHandle{};

	if (!HasAuthority())
	{
		ensure(false);
		return Handle;
	}

	UPMInventoryItemDefinition* ItemCDO = GetItemDefCDO(ItemRequest);
	if (!ItemCDO)
	{
		ensure(false);
		MCHAE_WARNING("ItemCDO is not valid");
		return Handle;
	}

	FPMInventoryItemList* ItemList = GetItemList(ItemCDO->ItemType);
	if (ItemList)
	{
		FPMInventoryEntry* Entry = ItemList->FindEntry(ItemCDO->GetClass());
		if (Entry && ItemList->OwnedItemType != EMItemType::Equipment &&
			(ItemRequest.RequestType == EMItemRequestType::AddItem || ItemRequest.RequestType == EMItemRequestType::RemoveItem))
		{
			Handle = Entry->GetItemHandle();
			ChangeItemQuantity(Handle, ItemRequest);
		}
		else if (ItemRequest.RequestType == EMItemRequestType::AddItem || ItemRequest.RequestType == EMItemRequestType::InitItem)
		{
			Handle = AddItemDefinition_Impl(ItemCDO->GetClass(), *ItemList, ItemRequest);
		}
		else if (ItemRequest.RequestType == EMItemRequestType::ReturnItemToInventory)
		{
			Handle = ReturnItem(ItemRequest.ItemInstance);
		}
	}

	return Handle;
}

FMItemHandle UPMInventoryManagerComponent::AddItemDefinition_Impl(TSubclassOf<UPMInventoryItemDefinition> ItemDef, FPMInventoryItemList& ItemList, const FMItemRequest& ItemRequest)
{
	FMItemHandle Handle = FMItemHandle{};

	Handle = ItemList.AddEntry(ItemDef, ItemRequest.ItemQuentity);

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

	FMItemResponse Response;
	Response.ResponsType = EMItemResponseType::TotallyNewItem;
	Response.ItemRequest = ItemRequest;
	Response.ItemQuentity = ItemInstance->GetItemQuentity();

	Multicast_OnNewItemAdded(*Entry, Response);

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

UPMInventoryItemDefinition* UPMInventoryManagerComponent::GetItemDefCDO(const int32 ItemRowId)
{
	UPMInventoryItemDefinition* ItemCDO = nullptr;

	UMDataTableManager* TableManager = GEngine->GetEngineSubsystem<UMDataTableManager>();
	if (TableManager)
	{
		TSubclassOf<UPMInventoryItemDefinition> Class = TableManager->GetDefinitionClass<UPMInventoryItemDefinition>(ItemRowId);

		if (Class)
		{
			ItemCDO = Class->GetDefaultObject<UPMInventoryItemDefinition>();
		}
	}

	return ItemCDO;
}

UPMInventoryItemDefinition* UPMInventoryManagerComponent::GetItemDefCDO(const TSubclassOf<UPMInventoryItemDefinition>& ItemDef)
{
	if (ItemDef)
	{
		return ItemDef->GetDefaultObject<UPMInventoryItemDefinition>();
	}

	return nullptr;
}

UPMInventoryItemDefinition* UPMInventoryManagerComponent::GetItemDefCDO(const FMItemRequest& ItemRequest)
{
	UPMInventoryItemDefinition* ItemCDO = nullptr;
	if (ItemRequest.ItemDef)
	{
		ItemCDO = GetItemDefCDO(ItemRequest.ItemDef);
	}

	if (!ItemCDO)
	{
		ItemCDO = GetItemDefCDO(ItemRequest.ItemRowId);
	}

	return ItemCDO;
}

void UPMInventoryManagerComponent::Multicast_OnNewItemAdded_Implementation(const FPMInventoryEntry& ItemEntry, const FMItemResponse& ItemRespons)
{
	Delegate_OnNewItemAdded.Broadcast(ItemEntry, ItemRespons);
}

void UPMInventoryManagerComponent::Multicast_OnChangeInventory_Implementation(const FMItemHandle& ItemHandle, const FMItemResponse& ItemRespons)
{
	FOnChangeItemQuentity* InventoryDelegates = Delegate_OnChangeItemQuentity.Find(ItemHandle.ItemUid);
	if (InventoryDelegates)
	{
		InventoryDelegates->Broadcast(ItemHandle, ItemRespons);
	}
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

int32 UPMInventoryManagerComponent::ChangeItemQuantity(const FMItemHandle& ItemHandle, const FMItemRequest& ItemRequest)
{
	FPMInventoryItemList* ItemList = GetItemList(ItemHandle.ItemType);
	if (ItemList)
	{
		int32 ItemQuentity = 0;
		if (ItemRequest.RequestType == EMItemRequestType::AddItem)
		{
			ItemQuentity = ItemRequest.ItemQuentity > 0 ? ItemRequest.ItemQuentity : -ItemRequest.ItemQuentity;
		}
		else if (ItemRequest.RequestType == EMItemRequestType::RemoveItem)
		{
			ItemQuentity = ItemRequest.ItemQuentity < 0 ? ItemRequest.ItemQuentity : -ItemRequest.ItemQuentity;
		}

		int32 CurrentItemQuentity = ItemList->ChangeItemQuantity(ItemHandle, ItemQuentity);

		if (CurrentItemQuentity == 0)
		{
			RemoveItem(ItemHandle);
		}
		else
		{
			FMItemResponse Response;

			Response.ItemRequest = ItemRequest;
			Response.ResponsType = EMItemResponseType::ChangeItemQuentity;
			Response.ItemQuentity = CurrentItemQuentity;
			Multicast_OnChangeInventory(ItemHandle, Response);
		}
		return CurrentItemQuentity;
	}

	return 0;
}

FDelegateHandle UPMInventoryManagerComponent::AddDelegateOnChangeItemQuentity(const int32 ItemUid, FOnChangeItemQuentity::FDelegate&& Delegate)
{
	FOnChangeItemQuentity& InventoryDelegates = Delegate_OnChangeItemQuentity.FindOrAdd(ItemUid);

	FDelegateHandle DelegateHandle = InventoryDelegates.Add(Delegate);

	return DelegateHandle;
}

void UPMInventoryManagerComponent::RemoveDelegateOnChangeItemQuentity(const int32 ItemUid, const FDelegateHandle& DelegateHandle)
{
	FOnChangeItemQuentity* InventoryDelegates = Delegate_OnChangeItemQuentity.Find(ItemUid);

	if (InventoryDelegates)
	{
		InventoryDelegates->Remove(DelegateHandle);
		
		if (InventoryDelegates->IsBound() == false)
		{
			Delegate_OnChangeItemQuentity.Remove(ItemUid);
		}
	}
}

void UPMInventoryManagerComponent::Server_UseItem_Implementation(const FMItemHandle& ItemHandle)
{
	UPMInventoryItemInstance* ItemInstance = FindItemInstance(ItemHandle);
	if (ItemInstance)
	{
		if (ItemInstance->CanUseItem())
		{
			ItemInstance->ActivateItem();
			FMItemRequest Request;
			Request.RequestType = EMItemRequestType::RemoveItem;
			Request.ItemQuentity = -1;
			Request.ItemHandle = ItemHandle;

			ChangeItemQuantity(ItemHandle, Request);
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
			Delegate_OnChangeItemQuentity.Remove(ItemHandle.ItemUid);
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

#if WITH_EDITOR
void UPMInventoryManagerComponent::Debug_AddItem(int32 RowId, int32 ItemQuentity)
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
						DebugServer_AddItem(Item->RowId, ItemQuentity);
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

void UPMInventoryManagerComponent::DebugServer_AddItem_Implementation(int32 Rowid, int32 ItemQuentity)
{
	FMItemRequest Request;
	Request.ItemRowId = Rowid;
	Request.ItemQuentity = ItemQuentity;
	Request.RequestType = EMItemRequestType::AddItem;

	RequestItemToInventory(Request);
}
#endif
