#include "PMEquipmentManagerComponent.h"
#include "Engine/Engine.h"
#include "PMEquipmentInstance.h"
#include "PMEquipmentDefinition.h"
#include "AbilitySystemGlobals.h"
#include "../AbilitySystem/PMAbilitySystemComponent.h"

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

/*
* FPMEquipmentList
*/
UPMEquipmentInstance* FPMEquipmentList::AddEntry(TSubclassOf<UPMEquipmentDefinition> EquipmentDefinition)
{
	UPMEquipmentInstance* Result = nullptr;
	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	const UPMEquipmentDefinition* EquipmentCDO = GetDefault<UPMEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UPMEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = UPMEquipmentInstance::StaticClass();
	}

	FPMAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UPMEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	Result = NewEntry.Instance;

	// 장비에서 보유중인 어빌리티 부여
	UPMAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);
	for (const UPMAbilitySet* AbilitySet : EquipmentCDO->AbilitySetsToGrant)
	{
		AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Result);
	}

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

	return Result;
}

void FPMEquipmentList::RemoveEntry(UPMEquipmentInstance* Instance)
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
}

UPMEquipmentInstance* UPMEquipmentManagerComponent::EquipItem(TSubclassOf<UPMEquipmentDefinition> EquipmentDefinition)
{
	UPMEquipmentInstance* Result = nullptr;
	if (EquipmentDefinition)
	{
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (Result)
		{
			Result->OnEquipped();
		}
	}

	return Result;
}

void UPMEquipmentManagerComponent::UnequipItem(UPMEquipmentInstance* ItemInstance)
{
	if (ItemInstance)
	{
		ItemInstance->OnUnequipped();

		EquipmentList.RemoveEntry(ItemInstance);
	}
}

TArray<UPMEquipmentInstance*> UPMEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UPMEquipmentInstance> InstanceType) const
{
	TArray<UPMEquipmentInstance*> Result;

	for (const FPMAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UPMEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Result.Add(Instance);
			}
		}
	}

	return Result;
}
