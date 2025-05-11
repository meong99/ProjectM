#include "PMWeaponInstance.h"
#include "Cosmetics/PMCosmeticAnimationTypes.h"
#include "Cosmetics/PMPawnComponent_CharacterParts.h"
#include "Animation/AnimInstance.h"
#include "Character/MPlayerCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Equipment/PMQuickBarComponent.h"
#include "Equipment/PMEquipmentManagerComponent.h"

UPMWeaponInstance::UPMWeaponInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

int32 UPMWeaponInstance::ActivateItem()
{
	int32 ItemNum = Super::ActivateItem();

	AActor* Controller = Cast<AActor>(GetOuter());
	UPMEquipmentManagerComponent* EquipmentManager = Controller ? Controller->FindComponentByClass<UPMEquipmentManagerComponent>() : nullptr;
	if (EquipmentManager)
	{
		EquipmentManager->EquipItem(this);
	}
	else
	{
		ensure(false);
		MCHAE_WARNING("EquipmentManager is not valid");
	}

	return ItemNum;
}

void UPMWeaponInstance::OnEquipped()
{
	DetermineCosmeticTag();
}

void UPMWeaponInstance::OnUnequipped()
{
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
