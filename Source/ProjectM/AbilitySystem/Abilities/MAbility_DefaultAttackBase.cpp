#include "MAbility_DefaultAttackBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Weapons/PMWeaponInstance.h"
#include "Weapons/MWeaponBase.h"

UMAbility_DefaultAttackBase::UMAbility_DefaultAttackBase()
{

}

void UMAbility_DefaultAttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec();
	UPMWeaponInstance* WeaponInstance = Cast<UPMWeaponInstance>(Spec->SourceObject);
	if (WeaponInstance)
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = nullptr;
		PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			   this, FName(GetName()), WeaponInstance->WeaponMontageToPlay);

		if (PlayMontageAndWait)
		{
			PlayMontageAndWait->OnCompleted.AddDynamic(this, &UMAbility_DefaultAttackBase::NotifyMontageCanceledCallBack);
			PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UMAbility_DefaultAttackBase::NotifyMontageCanceledCallBack);
			PlayMontageAndWait->OnCancelled.AddDynamic(this, &UMAbility_DefaultAttackBase::NotifyMontageCanceledCallBack);
			PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UMAbility_DefaultAttackBase::NotifyMontageCanceledCallBack);

			PlayMontageAndWait->ReadyForActivation();

			for (AActor* SpawnedActor : WeaponInstance->SpawnedActors)
			{
				AMWeaponBase* Weapon = Cast<AMWeaponBase>(SpawnedActor);
				if (Weapon)
				{
					Weapon->ActivateWeapon();
				}
			}
		}
		else
		{
			NotifyMontageCanceledCallBack();
		}
	}
}

void UMAbility_DefaultAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec();
	UPMWeaponInstance* WeaponInstance = Cast<UPMWeaponInstance>(Spec->SourceObject);
	if (WeaponInstance)
	{
		for (AActor* SpawnedActor : WeaponInstance->SpawnedActors)
		{
			AMWeaponBase* Weapon = Cast<AMWeaponBase>(SpawnedActor);
			if (Weapon)
			{
				Weapon->DeactivateWeapon();
			}
		}
	}
}

void UMAbility_DefaultAttackBase::NotifyMontageCanceledCallBack()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}
