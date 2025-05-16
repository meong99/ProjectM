#include "MAbility_MonsterAttackBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "System/MDataTableManager.h"
#include "Engine/Engine.h"
#include "Definitions/MMonsterDefinition.h"
#include "Indicator/MFanShapeIndicator.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Util/MGameplayStatics.h"
#include "Character/MPlayerCharacterBase.h"
#include "Table/Item/MTable_GameplayEffectTable.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "PMGameplayTags.h"
#include "Character/Monster/MMonsterBase.h"

UMAbility_MonsterAttackBase::UMAbility_MonsterAttackBase()
{
}

void UMAbility_MonsterAttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FGameplayAbilitySpec* Spec = GetCurrentAbilitySpec();
	UMDataTableManager* TableManager = GEngine->GetEngineSubsystem<UMDataTableManager>();
	MonsterDef = TableManager ? Cast<UMMonsterDefinition>(TableManager->GetMonsterDefinition(Spec->InputID)) : nullptr;

	if (MonsterDef)
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = nullptr;
		PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, FName(GetName()), MonsterDef->MonsterCombatInfo.MontageInfo.Montage, MonsterDef->MonsterCombatInfo.MontageInfo.Rate, MonsterDef->MonsterCombatInfo.MontageInfo.StartSection);

		if (PlayMontageAndWait)
		{
			PlayMontageAndWait->OnCompleted.AddDynamic(this, &UMAbility_MonsterAttackBase::CallBack_MontageEnded);
			PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UMAbility_MonsterAttackBase::CallBack_MontageEnded);
			PlayMontageAndWait->OnCancelled.AddDynamic(this, &UMAbility_MonsterAttackBase::CallBack_MontageEnded);
			PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UMAbility_MonsterAttackBase::CallBack_MontageEnded);

			PlayMontageAndWait->ReadyForActivation();
		}

		if (HasAuthority(&ActivationInfo))
		{
			TraceAttack();
		}
	}
}

void UMAbility_MonsterAttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMAbility_MonsterAttackBase::TraceAttack()
{
	AMMonsterBase* OwnerActor = Cast<AMMonsterBase>(GetActorInfo().OwnerActor.Get());
	if (OwnerActor)
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { UEngineTypes::ConvertToObjectType(UMGameplayStatics::GetCollisionChannel(EMCollisionChannel::Player)) };
		TArray<AActor*> ActorsToIgnore = { OwnerActor };
		EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;
		if (UMGameplayStatics::bShowDebug_Console)
		{
			DebugType = EDrawDebugTrace::ForDuration;
		}
		TArray<FHitResult> OutHits;
		UKismetSystemLibrary::SphereTraceMultiForObjects(this, OwnerActor->GetActorLocation(), 
														 OwnerActor->GetActorLocation() + OwnerActor->GetActorForwardVector() * MonsterDef->MonsterCombatInfo.MonsterDefaultAttackRange,
														 MonsterDef->MonsterCombatInfo.MonsterDefaultAttackRadius, ObjectTypes, false, ActorsToIgnore, DebugType, OutHits, true);

		for (const FHitResult& Hit : OutHits)
		{
			AMPlayerCharacterBase* Player = Cast<AMPlayerCharacterBase>(Hit.GetActor());
			UPMAbilitySystemComponent* MonsterASC = OwnerActor ? OwnerActor->GetMAbilitySystemComponent() : nullptr;
			if (MonsterASC)
			{
				FMTable_GameplayEffectTable* EffectRow = UMDataTableManager::GetTableRowData<FMTable_GameplayEffectTable>(this, 21000001);
				if (EffectRow)
				{
					TMap<FGameplayTag, float> SetbyCallerMap;
					#pragma TODO("공격력 적용해야함")
					SetbyCallerMap.Add(FPMGameplayTags::Get().Ability_Effect_SetByCaller_Health, -10/*이거 공격력으로*/);
					MonsterASC->ApplyEffectToTargetWithSetByCaller(EffectRow->GameplayEffect, Player, OwnerActor, SetbyCallerMap);
				}
			}
		}
	}
	else
	{
		ensure(false);
	}
}

void UMAbility_MonsterAttackBase::CallBack_MontageEnded()
{
	const FGameplayAbilityActivationInfo ActivationInfo = GetCurrentActivationInfo();
	if (HasAuthority(&ActivationInfo))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
}
