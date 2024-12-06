#include "PMInventoryItemManagerComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "PMInventoryItemInstance.h"

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

	Result = NewEntry.Instance;
	return Result;
}

/*
* UPMInventoryItemManagerComponent -------------------------------
*/
UPMInventoryItemManagerComponent::UPMInventoryItemManagerComponent(const FObjectInitializer& ObjectInitializer)
	: InventoryList(this)
{
}

UPMInventoryItemInstance* UPMInventoryItemManagerComponent::AddItemDefinition(TSubclassOf<UPMInventoryItemDefinition> ItemDef)
{
	UPMInventoryItemInstance* Result = nullptr;
	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);
	}

	return Result;
}
