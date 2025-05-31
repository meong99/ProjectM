#include "PMEquipmentManagerComponent.h"
#include "Engine/Engine.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "System/MDataTableManager.h"
#include "Table/Item/MTable_EquipmentItem.h"
#include "Inventory/PMInventoryManagerComponent.h"
#include "PMQuickBarComponent.h"
#include "Item/Equipment/MEquipableItemInstance.h"
#include "Item/Equipment/MEquipmentItemDefinition.h"
#include "AbilitySystem/PMAbilitySet.h"
#include "Player/PMPlayerControllerBase.h"
#include "Item/Equipment/MEquipmentItemInstance.h"
#include "Player/PMPlayerState.h"
#include "Character/PMPawnData.h"

/*
* UPMEquipmentManagerComponent
*/
UPMEquipmentManagerComponent::UPMEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquippedItemList(this)
{
	SetIsReplicatedByDefault(true);
}

void UPMEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquippedItemList);
}

void UPMEquipmentManagerComponent::BeginDestroy()
{
	Super::BeginDestroy();

	UnequipAllItems();
}

void UPMEquipmentManagerComponent::OnPossess(APawn* aPawn)
{
	if (!bIsInitialized)
	{
		bIsInitialized = true;
		EquipDefaultWeapon();
	}
}

void UPMEquipmentManagerComponent::OnServerRestartPlayer()
{
	UnequipAllItems();
	EquipAllItems();
}

void UPMEquipmentManagerComponent::EquipItem(const int32 ItemRowId)
{
	const UMEquipmentItemDefinition* EquipDef = UMDataTableManager::GetDefinitionObject<UMEquipmentItemDefinition>(this, ItemRowId);
	if (EquipDef)
	{
		if (FindEquippedItemInstance(EquipDef->EquipmentItemType))
		{
			UnequipItem(EquipDef->EquipmentItemType);
		}

		EquipItem_Impl(EquipDef);
	}
	else
	{
		ensure(false);
		MCHAE_ERROR("장비 장착 중 Definition을 찾지 못 함");
	}
}

void UPMEquipmentManagerComponent::UnequipItem(EMEquipmentItemType EquipmentItemType)
{
	UMEquipmentItemInstance* UnEquippedItem = FindEquippedItemInstance(EquipmentItemType);

	if (UnEquippedItem)
	{
		AActor* Controller = GetOwner();
		UPMInventoryManagerComponent* InvenManager = Controller ? Controller->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
		if (InvenManager)
		{
			FMItemRequest Request;
			Request.SetItemRequest(EMItemRequestType::ReturnItemToInventory, UnEquippedItem->ItemRowId, 1, UnEquippedItem->ItemHandle);
			InvenManager->RequestItemToInventory(Request);
		}

		UPMAbilitySystemComponent* ASC = GetAbilitySystemComponent();
		if (ASC == nullptr)
		{
			ensure(false);
			MCHAE_ERROR("Can't access to abilitysystem when equip item. So item equip sequence will be canceled.");
			return;
		}

		FMAbilitySet_GrantedHandles TempGrantedHandles;
		GrantedHandles.RemoveAndCopyValue(UnEquippedItem->ItemHandle.ItemUid, TempGrantedHandles);
		TempGrantedHandles.TakeFromAbilitySystem(ASC);

		UnEquippedItem->OnUnequipped();
		RemoveEntry(EquipmentItemType);
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(UnEquippedItem);
		}
	}
}

UMEquipmentItemInstance* UPMEquipmentManagerComponent::FindEquippedItemInstance(EMEquipmentItemType EquipmentItemType)
{
	FMEquipmentItemEntry* Entry = FindEntry(EquipmentItemType);
	UMEquipmentItemInstance* Instance = nullptr;
	if (Entry)
	{
		Instance = Entry->Instance;
	}

	return Instance;
}

FMEquipmentItemEntry* UPMEquipmentManagerComponent::FindEntry(EMEquipmentItemType EquipmentItemType)
{
	FMItemHandle Handle = EquippedEntryMap.FindRef(EquipmentItemType);
	return EquippedItemList.FindEntry(Handle);
}

void UPMEquipmentManagerComponent::EquipItem_Impl(const UMEquipmentItemDefinition* EquipDef)
{
	UPMAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ASC == nullptr)
	{
		ensure(false);
		MCHAE_ERROR("Can't access to abilitysystem when equip item. So item equip sequence will be canceled.");
		return;
	}

	FMItemHandle Handle = EquippedItemList.AddEntry(EquipDef->GetClass(), 1);
	FMEquipmentItemEntry* Entry = EquippedItemList.FindEntry(Handle);
	if (Entry && Entry->Instance)
	{
		FMAbilitySet_GrantedHandles TempGrantedHandles;
		for (const UPMAbilitySet* AbilitySet : EquipDef->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, &TempGrantedHandles, Entry->Instance, EquipDef->RowId);
		}

		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Entry->Instance)
		{
			AddReplicatedSubObject(Entry->Instance);
		}

		GrantedHandles.Add(Entry->Instance->ItemHandle.ItemUid, TempGrantedHandles);
		EquippedEntryMap.Add(Entry->Instance->EquipmentItemType, Entry->GetItemHandle());
		Entry->Instance->OnEquipped();
	}
}

void UPMEquipmentManagerComponent::RemoveEntry(EMEquipmentItemType EquipmentItemType)
{
	FMItemHandle Handle = EquippedEntryMap.FindRef(EquipmentItemType);
	EquippedItemList.RemoveEntry(Handle);
}

void UPMEquipmentManagerComponent::EquipAllItems()
{
	for (FMEquipmentItemEntry& Entry : EquippedItemList.Entries)
	{
		UMEquipmentItemInstance* Instance = Entry.Instance;
		if (IsValid(Instance))
		{
			const UMEquipmentItemDefinition* ItemCDO = Instance->ItemDef->GetDefaultObject<UMEquipmentItemDefinition>();
			if (ItemCDO)
			{
				Instance->OnEquipped();
			}
		}
	}
}

void UPMEquipmentManagerComponent::UnequipAllItems()
{
	for (FMEquipmentItemEntry& Entry : EquippedItemList.Entries)
	{
		UMEquipmentItemInstance* Instance = Entry.Instance;
		if (IsValid(Instance))
		{
			Instance->OnUnequipped();
		}
	}
}

void UPMEquipmentManagerComponent::EquipDefaultWeapon()
{
	APMPlayerControllerBase* Controller = GetOwner<APMPlayerControllerBase>();
	APMPlayerState* PlayerState = Controller ? Controller->GetPlayerState() : nullptr;
	if (PlayerState)
	{
		const UPMPawnData* PawnData = PlayerState->GetPawnData();
		if (PawnData)
		{
			EquipItem(PawnData->DefaultEquipmentRowId);
		}
	}
}

UPMAbilitySystemComponent* UPMEquipmentManagerComponent::GetAbilitySystemComponent() const
{
	APMPlayerControllerBase* Controller = GetOwner<APMPlayerControllerBase>();
	if (Controller)
	{
		return Controller->GetAbilitySystemComponent();
	}

	return nullptr;
}
