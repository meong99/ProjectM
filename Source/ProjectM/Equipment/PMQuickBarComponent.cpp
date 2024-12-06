#include "PMQuickBarComponent.h"
#include "PMEquipmentManagerComponent.h"
#include "../Inventory/PMInventoryFragment_EquippableItem.h"
#include "PMEquipmentInstance.h"
#include "../Inventory/PMInventoryItemInstance.h"

UPMQuickBarComponent::UPMQuickBarComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/) : Super(ObjectInitializer)
{
}

void UPMQuickBarComponent::BeginPlay()
{
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}

	Super::BeginPlay();
}

void UPMQuickBarComponent::AddItemToSlot(const int32 SlotIndex, UPMInventoryItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && Item)
	{
		if (!Slots[SlotIndex])
		{
			Slots[SlotIndex] = Item;
		}
	}
}

void UPMQuickBarComponent::SetActiveSlotIndex(const int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && ActiveSlotIndex != NewIndex)
	{
		UnequipItemInSlot();
		ActiveSlotIndex = NewIndex;
		EquipItemInSlot();
	}
}

UPMEquipmentManagerComponent* UPMQuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<UPMEquipmentManagerComponent>();
		}
	}

	return nullptr;
}

void UPMQuickBarComponent::UnequipItemInSlot()
{
	if (UPMEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		if (EquippedItem)
		{
			EquipmentManager->UnequipItem(EquippedItem);
			EquippedItem = nullptr;
		}
	}
}

void UPMQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	if (UPMInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		if (const UPMInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UPMInventoryFragment_EquippableItem>())
		{
			TSubclassOf<UPMEquipmentDefinition> EquipDef = EquipInfo->GetEquipmentDefinition();
			if (EquipDef)
			{
				if (UPMEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);
					if (EquippedItem)
					{
						EquippedItem->SetInstigator(SlotItem);
					}
				}
			}
		}
	}
}