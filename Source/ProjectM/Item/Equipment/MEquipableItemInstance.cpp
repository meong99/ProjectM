#include "MEquipableItemInstance.h"
#include "PMGameplayTags.h"
#include "Player/PMPlayerControllerBase.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "MEquipmentItemDefinition.h"
#include "Equipment/MEquipableActorBase.h"
#include "GameplayTagContainer.h"
#include "Equipment/PMEquipmentManagerComponent.h"

UMEquipableItemInstance::UMEquipableItemInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMEquipableItemInstance::OnInstanceCreated()
{
	UMEquipmentItemDefinition* ItemCDO = ItemDef->GetDefaultObject<UMEquipmentItemDefinition>();
	if (ItemCDO)
	{
		EquipmentItemType = ItemCDO->EquipmentItemType;
	}
}

bool UMEquipableItemInstance::ActivateItem()
{
	const bool bIsActivated = Super::ActivateItem();

	if (bIsActivated)
	{
		AActor* Controller = Cast<AActor>(GetOuter());
		UPMEquipmentManagerComponent* EquipmentManager = Controller ? Controller->FindComponentByClass<UPMEquipmentManagerComponent>() : nullptr;
		if (EquipmentManager)
		{
			EquipmentManager->EquipItem(ItemRowId);
		}
		else
		{
			ensure(false);
			MCHAE_WARNING("EquipmentManager is not valid");
		}
	}

	return bIsActivated;
}

bool UMEquipableItemInstance::CanUseItem() const
{
	bool bCanUseItem = false;

	if (GetStatTagStackCount(FPMGameplayTags::Get().Item_Quentity))
	{
		bCanUseItem = true;
	}
	if (GetAbilitySystemComponent())
	{
		bCanUseItem &= true;
	}

	return bCanUseItem;
}

APawn* UMEquipableItemInstance::GetPawn() const
{
	APlayerController* Controller = Cast<APlayerController>(GetOuter());
	if (Controller)
	{
		return Controller->GetPawn();
	}

	return nullptr;
}

APawn* UMEquipableItemInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		APawn* CurrentPawn = GetPawn();
		if (CurrentPawn && CurrentPawn->IsA(ActualPawnType))
		{
			Result = CurrentPawn;
		}
	}

	return Result;
}

UPMAbilitySystemComponent* UMEquipableItemInstance::GetAbilitySystemComponent() const
{
	APMPlayerControllerBase* Controller = Cast<APMPlayerControllerBase>(GetOuter());
	if (Controller)
	{
		return Controller->GetAbilitySystemComponent();
	}

	return nullptr;
}
