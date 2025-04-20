#include "PMWeaponInstance.h"
#include "Cosmetics/PMCosmeticAnimationTypes.h"
#include "Cosmetics/PMPawnComponent_CharacterParts.h"
#include "Animation/AnimInstance.h"
#include "Character/MPlayerCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Equipment/PMQuickBarComponent.h"

UPMWeaponInstance::UPMWeaponInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

int32 UPMWeaponInstance::UseItem()
{
	int32 ItemNum = Super::UseItem();
	OnUnequipped();
	OnEquipped();
	return ItemNum;
}

void UPMWeaponInstance::OnEquipped()
{
	DetermineCosmeticTag();

	#pragma TODO("Quickbar사용하지 않고 장착할 수 있어야함")
	AActor* Outer = Cast<AActor>(GetOuter());
	UPMQuickBarComponent* QuickBarComp = Outer ? Outer->FindComponentByClass<UPMQuickBarComponent>() : nullptr;
	if (QuickBarComp)
	{
		QuickBarComp->AddItemToSlot(0, this);
		QuickBarComp->SetActiveSlotIndex(0);
	}
}

void UPMWeaponInstance::OnUnequipped()
{
	AActor* Outer = Cast<AActor>(GetOuter());
	UPMQuickBarComponent* QuickBarComp = Outer ? Outer->FindComponentByClass<UPMQuickBarComponent>() : nullptr;
	if (QuickBarComp && QuickBarComp->GetActiveSlotIndex() != INDEX_NONE)
	{
		QuickBarComp->UnequipItemInSlot();
	}
}

void UPMWeaponInstance::DetermineCosmeticTag()
{
	APawn* Pawn = GetPawn();
	UPMPawnComponent_CharacterParts* CharacterPart = Pawn ? Pawn->FindComponentByClass<UPMPawnComponent_CharacterParts>() : nullptr;
	if (IsValid(CharacterPart))
	{
		CosmeticAnimStyle = CharacterPart->GetCombinedTags(FGameplayTag{});
		const TSubclassOf<UAnimInstance>& AnimInstance = PickBestAnimLayer(true, CosmeticAnimStyle);
		if (AnimInstance)
		{
			AMPlayerCharacterBase* Character = Cast<AMPlayerCharacterBase>(GetTypedPawn(AMPlayerCharacterBase::StaticClass()));
			if (Character)
			{
				USkeletalMeshComponent* Mesh = Character->GetMesh();
				if (Mesh)
				{
					Mesh->LinkAnimClassLayers(AnimInstance);
					Character->PlayAnimMontage(WeaponMontageToPlay);
				}
			}
		}
	}
}

TSubclassOf<UAnimInstance> UPMWeaponInstance::PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const
{
	const FPMAnimInstanceSelectionSet& setToQuery = bEquipped ? EquippedAnimSet : UnEquippedAnimSet;

	return setToQuery.SelectBestAnimInstance(CosmeticTags);
}
