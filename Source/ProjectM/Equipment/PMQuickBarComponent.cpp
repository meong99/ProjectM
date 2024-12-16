#include "PMQuickBarComponent.h"
#include "PMEquipmentManagerComponent.h"
#include "Inventory/Fragments/PMInventoryFragment_EquippableItem.h"
#include "PMEquipmentInstance.h"
#include "Inventory/PMInventoryItemInstance.h"

#pragma TODO("퀵바를 RPG스럽게 다룰 수 있는지 확인해보고, 추후 기능들을 추가할 때 한 번 더 봐보자")

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

	if (UPMInventoryItemInstance* SlotItemInstance = Slots[ActiveSlotIndex])
	{
		if (const UPMInventoryFragment_EquippableItem* EquipInfo = SlotItemInstance->FindFragmentByClass<UPMInventoryFragment_EquippableItem>())
		{
			TSubclassOf<UPMEquipmentDefinition> EquipDef = EquipInfo->GetEquipmentDefinition();
			if (EquipDef)
			{
				if (UPMEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);
					if (EquippedItem)
					{
						EquippedItem->SetInstigator(SlotItemInstance);
					}
				}
			}
		}
	}
}