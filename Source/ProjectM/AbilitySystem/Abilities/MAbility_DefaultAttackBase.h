// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PMGameplayAbility.h"
#include "MAbility_DefaultAttackBase.generated.h"

class ACharacter;
class AActor;
class UMWeaponItemDefinition;
class UPMWeaponInstance;
class UAbilityTask;

UCLASS()
class PROJECTM_API UMAbility_DefaultAttackBase : public UPMGameplayAbility
{
	GENERATED_BODY()

	/*
	* Overrided Functions
	*/
public:
	UMAbility_DefaultAttackBase();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

/*
* Member Functions
*/
public:

protected:
	UFUNCTION()
	virtual void StartAttackTracing(FGameplayEventData Payload);
	UFUNCTION()
	virtual void EndAttackTracing(FGameplayEventData Payload);

	virtual void TraceAttack(ACharacter* OwnerCharacter, UPMWeaponInstance* WeaponInstance);
	virtual void Callback_OnHit(const TArray<AActor*>& HitActors);

	UFUNCTION()
	void NotifyMontageEndCallBack();

/*
* Member Variables
*/
protected:
	UPROPERTY();
	UMWeaponItemDefinition* ItemDef;

	UPROPERTY()
	TSet<AActor*> OverlappedActors;

	UPROPERTY()
	TObjectPtr<UAbilityTask> TraceTask;

	int32 MontageIndex = 0;
};