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

FPMEquipmentList::FPMEquipmentList()
{
	if (GEngine && GEngine->GetWorld() && GEngine->GetWorld()->GetGameInstance()
		&& IsValid(OwnerComponent) == false)
	{
		MCHAE_FETAL("OwnerComponent must be set!!");
	}
}

FPMEquipmentList::FPMEquipmentList(UActorComponent* InOwnerComponent)
	: OwnerComponent(InOwnerComponent)
{
	if (IsValid(OwnerComponent) == false)
	{
		MCHAE_FETAL("OwnerComponent must be set!!");
	}
}

void FPMEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FPMAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
		}
	}
}

void FPMEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FPMAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
// 			Entry.Instance->OnEquipped();
		}
	}
}

void FPMEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
// 	for (int32 Index : ChangedIndices)
// 	{
// 		const FGameplayTagStack& Stack = Stacks[Index];
// 		TagToCountMap[Stack.Tag] = Stack.StackCount;
// 	}
}

/*
* FPMEquipmentList
*/
UMEquipmentItemInstance* FPMEquipmentList::AddEntry(TSubclassOf<UMEquipmentItemDefinition> EquipmentDefinition)
{
	UMEquipmentItemInstance* Result = nullptr;
	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	const UMEquipmentItemDefinition* EquipmentCDO = GetDefault<UMEquipmentItemDefinition>(EquipmentDefinition);

	TSubclassOf<UPMInventoryItemInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = UMEquipmentItemInstance::StaticClass();
	}

	FPMAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UMEquipmentItemInstance>(OwnerComponent->GetOwner(), InstanceType);
	for (const UPMInventoryItemFragment* Fragment : GetDefault<UMEquipmentItemDefinition>(EquipmentDefinition)->GetFragments())
	{
		if (Fragment)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	Result = NewEntry.Instance;

	MarkItemDirty(NewEntry);

	// 장비에서 보유중인 어빌리티 부여
	UPMAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ASC == nullptr)
	{
		MCHAE_ERROR("ASC is null");
		return nullptr;
	}

	for (const UPMAbilitySet* AbilitySet : EquipmentCDO->AbilitySetsToGrant)
	{
		AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Result);
	}

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

	return Result;
}

void FPMEquipmentList::RemoveEntry(UMEquipmentItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FPMAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			UPMAbilitySystemComponent* ASC = GetAbilitySystemComponent();
			check(ASC);
			Entry.GrantedHandles.TakeFromAbilitySystem(ASC);

			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

UPMAbilitySystemComponent* FPMEquipmentList::GetAbilitySystemComponent()
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();

	return Cast<UPMAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

/*
* UPMEquipmentManagerComponent
*/
UPMEquipmentManagerComponent::UPMEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this) // 이것도 필수!!!
{
	SetIsReplicatedByDefault(true);
}

bool UPMEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FPMAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		UMEquipmentItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UPMEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing LyraEquipmentInstances
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FPMAppliedEquipmentEntry& Entry : EquipmentList.Entries)
		{
			UMEquipmentItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

void UPMEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

UMEquipmentItemInstance* UPMEquipmentManagerComponent::EquipItem(TSubclassOf<UMEquipmentItemDefinition> EquipmentDefinition)
{
	UMEquipmentItemInstance* Result = nullptr;
	if (EquipmentDefinition)
	{
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (Result)
		{
			Result->OnEquipped();

			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}

	return Result;
}

void UPMEquipmentManagerComponent::UnequipItem(UMEquipmentItemInstance* ItemInstance)
{
	if (ItemInstance)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}

		ItemInstance->OnUnequipped();

		EquipmentList.RemoveEntry(ItemInstance);
	}
}

TArray<UMEquipmentItemInstance*> UPMEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UMEquipmentItemInstance> InstanceType) const
{
	TArray<UMEquipmentItemInstance*> Result;

	for (const FPMAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UMEquipmentItemInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Result.Add(Instance);
			}
		}
	}

	return Result;
}

UMEquipmentItemInstance* UPMEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<UMEquipmentItemInstance> InstanceType)
{
	for (FPMAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UMEquipmentItemInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}
	return nullptr;
}
