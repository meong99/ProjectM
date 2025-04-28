#include "MAbility_DefaultAttackBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Weapons/PMWeaponInstance.h"
#include "Weapons/MWeaponBase.h"
#include "System/MDataTableManager.h"
#include "Engine/Engine.h"
#include "Item/Equipment/MWeaponItemDefinition.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystem/AbilityTasks/MAbilityTask_CapsuleTracer.h"
#include "Character/Monster/MMonsterBase.h"
#include "PMGameplayTags.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"

UMAbility_DefaultAttackBase::UMAbility_DefaultAttackBase()
{

}

void UMAbility_DefaultAttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec();
	UMDataTableManager* TableManager = GEngine->GetEngineSubsystem<UMDataTableManager>();
	ItemDef = TableManager ? Cast<UMWeaponItemDefinition>(TableManager->GetItemDefinition(Spec->InputID)) : nullptr;

	if (ItemDef)
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = nullptr;
		PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			   this, FName(GetName()), ItemDef->DefaultAttackMontage);

		if (PlayMontageAndWait)
		{
			PlayMontageAndWait->OnCompleted.AddDynamic(this, &UMAbility_DefaultAttackBase::NotifyMontageCanceledCallBack);
			PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UMAbility_DefaultAttackBase::NotifyMontageCanceledCallBack);
			PlayMontageAndWait->OnCancelled.AddDynamic(this, &UMAbility_DefaultAttackBase::NotifyMontageCanceledCallBack);
			PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UMAbility_DefaultAttackBase::NotifyMontageCanceledCallBack);

			PlayMontageAndWait->ReadyForActivation();
		}
		else
		{
			NotifyMontageCanceledCallBack();
		}

		UPMWeaponInstance* WeaponInstance = Cast<UPMWeaponInstance>(Spec->SourceObject);
		if (HasAuthority(&ActivationInfo) && WeaponInstance)
		{
			ACharacter* OwnerCharacter = Cast<ACharacter>(WeaponInstance->GetPawn());
			if (OwnerCharacter)
			{
				TraceAttack(OwnerCharacter);
			}
		}
	}
	else
	{
		NotifyMontageCanceledCallBack();
	}
}

void UMAbility_DefaultAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	OverlappedActors.Empty();
	ItemDef = nullptr;
}

void UMAbility_DefaultAttackBase::TraceAttack(ACharacter* OwnerCharacter)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1) };
	TArray<AActor*> ActorsToIgnore = { OwnerCharacter };
	EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;
	TArray<FHitResult> OutHits;

	UMAbilityTask_CapsuleTracer* Task = UMAbilityTask_CapsuleTracer::CreateCapsuleTracerTask(
		this, 
		OwnerCharacter, 
		TEXT("weapon_start_r"),
		TEXT("weapon_end_r"),
		ObjectTypes, 
		FOnHit::TDelegate::CreateUObject(this, &UMAbility_DefaultAttackBase::Callback_OnHit), 
		ActorsToIgnore
	);

	if (Task)
	{
		Task->ReadyForActivation();
	}
	else
	{
		ensure(false);
		MCHAE_WARNING("Can't create CapsuleTracer!!");
	}
}

void UMAbility_DefaultAttackBase::Callback_OnHit(const TArray<AActor*>& HitActors)
{
	for (AActor* HitActor : HitActors)
	{
		AMMonsterBase* Monster = Cast<AMMonsterBase>(HitActor);
		if (Monster && !OverlappedActors.Contains(HitActor) && ItemDef)
		{
			UPMAbilitySystemComponent* OwnerAbilitySystem = Cast<UPMAbilitySystemComponent>(GetCurrentActorInfo()->AbilitySystemComponent);
			UMWeaponItemDefinition* WeaponDefCDO = Cast<UMWeaponItemDefinition>(ItemDef);

			if (OwnerAbilitySystem && WeaponDefCDO)
			{
				TMap<FGameplayTag, float> SetbyCallerMap;
	#pragma TODO("공격력 적용해야함")
				SetbyCallerMap.Add(FPMGameplayTags::Get().Ability_Effect_SetByCaller_Health, -10/*이거 공격력으로*/);
				OwnerAbilitySystem->ApplyEffectToTargetWithSetByCaller(WeaponDefCDO->DefaultAttackEffectClass, Monster, OwnerAbilitySystem->GetOwner(), SetbyCallerMap);
			}

			OverlappedActors.Add(HitActor);
		}
	}
}

void UMAbility_DefaultAttackBase::NotifyMontageCanceledCallBack()
{
	const FGameplayAbilityActivationInfo ActivationInfo = GetCurrentActivationInfo();
	if (HasAuthority(&ActivationInfo))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}
