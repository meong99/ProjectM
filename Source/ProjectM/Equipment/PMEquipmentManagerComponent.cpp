#include "PMEquipmentManagerComponent.h"
#include "Engine/Engine.h"
#include "PMEquipmentInstance.h"
#include "PMEquipmentDefinition.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "System/MDataTableManager.h"
#include "Table/Item/MTable_EquipmentItem.h"
#include "Inventory/PMInventoryManagerComponent.h"
#include "PMQuickBarComponent.h"
#include "Item/Equipment/MEquipmentItemInstance.h"
#include "Item/Equipment/MEquipmentItemDefinition.h"
#include "AbilitySystem/PMAbilitySet.h"
#include "Player/PMPlayerControllerBase.h"

/*
* UPMEquipmentManagerComponent
*/
UPMEquipmentManagerComponent::UPMEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UPMEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquippedItems);
}

void UPMEquipmentManagerComponent::BeginDestroy()
{
	Super::BeginDestroy();

	for (UMEquipmentItemInstance* Instance : EquippedItems)
	{
		if (IsValid(Instance))
		{
			Instance->DestroyEquipmentActors();
		}
	}
}

void UPMEquipmentManagerComponent::EquipItem(UMEquipmentItemInstance* ItemInstance)
{
	if (IsValid(ItemInstance))
	{
		const UMEquipmentItemDefinition* ItemCDO = ItemInstance->ItemDef->GetDefaultObject<UMEquipmentItemDefinition>();
		if (ItemCDO)
		{
			if (FindEquippedItem((int32)ItemInstance->GetEquipmentItemType()))
			{
				UnequipItem((int32)ItemInstance->GetEquipmentItemType());
			}

			UPMAbilitySystemComponent* ASC = GetAbilitySystemComponent();
			if (ASC == nullptr)
			{
				ensure(false);
				MCHAE_ERROR("Can't access to abilitysystem when equip item. So item equip sequence will be canceled.");
				return ;
			}

			FPMAbilitySet_GrantedHandles TempGrantedHandles;
			for (const UPMAbilitySet* AbilitySet : ItemCDO->AbilitySetsToGrant)
			{
				AbilitySet->GiveToAbilitySystem(ASC, &TempGrantedHandles, ItemInstance, ItemCDO->RowId);
			}

			GrantedHandles.Add(ItemInstance->ItemHandle.ItemUid, TempGrantedHandles);

			ItemInstance->SpawnEquipmentActors(ItemCDO->ActorsToSpawn);
			EquippedItems.Add(ItemInstance);
			OnRep_OnChangeEquipedItem();
		}
	}
}

void UPMEquipmentManagerComponent::UnequipItem(int32 EquipmentItemType)
{
	UMEquipmentItemInstance* UnEquippedItem = nullptr;

	int32 i = 0;
	for (; i < EquippedItems.Num(); i++)
	{
		if (EquippedItems[i] && EquipmentItemType == (int32)EquippedItems[i]->GetEquipmentItemType())
		{
			UnEquippedItem = EquippedItems[i];
			break;
		}
	}

	if (UnEquippedItem)
	{
		AActor* Controller = GetOwner();
		UPMInventoryManagerComponent* InvenManager = Controller ? Controller->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
		if (InvenManager)
		{
			InvenManager->ReturnItem(UnEquippedItem);
		}

		UPMAbilitySystemComponent* ASC = GetAbilitySystemComponent();
		if (ASC == nullptr)
		{
			ensure(false);
			MCHAE_ERROR("Can't access to abilitysystem when equip item. So item equip sequence will be canceled.");
			return ;
		}
		FPMAbilitySet_GrantedHandles TempGrantedHandles;
		GrantedHandles.RemoveAndCopyValue(UnEquippedItem->ItemHandle.ItemUid, TempGrantedHandles);
		TempGrantedHandles.TakeFromAbilitySystem(ASC);

		UnEquippedItem->OnUnequipped();
		UnEquippedItem->DestroyEquipmentActors();

		EquippedItems.RemoveAt(i);

		OnRep_OnChangeEquipedItem();
	}

}

UMEquipmentItemInstance* UPMEquipmentManagerComponent::FindEquippedItem(int32 EquipmentItemType)
{
	EMEquipmentItemType ItemType = (EMEquipmentItemType)EquipmentItemType;

	for (UMEquipmentItemInstance* Instance : EquippedItems)
	{
		if (Instance && Instance->GetEquipmentItemType() == ItemType)
		{
			return Instance;
		}
	}

	return nullptr;
}

void UPMEquipmentManagerComponent::OnRep_OnChangeEquipedItem()
{

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
