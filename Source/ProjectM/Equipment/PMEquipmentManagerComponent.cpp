#include "PMEquipmentManagerComponent.h"
#include "Engine/Engine.h"
#include "PMEquipmentInstance.h"
#include "PMEquipmentDefinition.h"

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
			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
	}
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
