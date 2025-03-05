#include "PMWeaponInstance.h"
#include "Cosmetics/PMCosmeticAnimationTypes.h"
#include "Cosmetics/PMPawnComponent_CharacterParts.h"
#include "Animation/AnimInstance.h"
#include "Character/PMCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"

UPMWeaponInstance::UPMWeaponInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
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
			APMCharacterBase* Character = Cast<APMCharacterBase>(GetTypedPawn(APMCharacterBase::StaticClass()));
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
