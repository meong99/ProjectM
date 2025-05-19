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
#include "AbilitySystem/AbilityTasks/MAbilityTask_SphereTracer.h"
#include "Character/Monster/MMonsterBase.h"
#include "PMGameplayTags.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "Equipment/PMEquipmentManagerComponent.h"
#include "AbilitySystem/Attributes/PMCombatSet.h"

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
				TraceAttack(OwnerCharacter, WeaponInstance);
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

void UMAbility_DefaultAttackBase::TraceAttack(ACharacter* OwnerCharacter, UPMWeaponInstance* WeaponInstance)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1) };
	TArray<AActor*> ActorsToIgnore = { OwnerCharacter };
	EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;
	TArray<FHitResult> OutHits;

	AActor* Weapon = nullptr;
	if (WeaponInstance->GetSpawnedActors().Num() > 0)
	{
		Weapon = WeaponInstance->GetSpawnedActors()[0];
	}

	if (!Weapon || !Weapon->FindComponentByClass<USkeletalMeshComponent>())
	{
		ensure(false);
		MCHAE_WARNING("Ability trace fail!! Because can't found weapon actor! Weapon actor must be spawned! ");
		return;
	}

	UMAbilityTask_SphereTracer* Task = UMAbilityTask_SphereTracer::CreateSphereTracerTask(
		this, 
		Weapon->FindComponentByClass<USkeletalMeshComponent>(),
		30.f,
		TEXT("weapon_start"),
		TEXT("weapon_end"),
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
		MCHAE_WARNING("Can't create SphereTracer!!");
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
			UPMAbilitySystemComponent* MonsterAbilitySystem = Monster->GetMAbilitySystemComponent();
			UMWeaponItemDefinition* WeaponDefCDO = Cast<UMWeaponItemDefinition>(ItemDef);
			const UPMCombatSet* CombatSet = OwnerAbilitySystem->GetSet<UPMCombatSet>();
			const UPMCombatSet* MonsterCombatSet = MonsterAbilitySystem ? MonsterAbilitySystem->GetSet<UPMCombatSet>() : nullptr;

			if (OwnerAbilitySystem && WeaponDefCDO && CombatSet && MonsterCombatSet)
			{
				TMap<FGameplayTag, float> SetbyCallerMap;
				SetbyCallerMap.Add(FPMGameplayTags::Get().Ability_Effect_SetByCaller_AttackPower, CombatSet->GetAttackPower());
				SetbyCallerMap.Add(FPMGameplayTags::Get().Ability_Effect_SetByCaller_DefensePower, MonsterCombatSet->GetDefensePower());
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
