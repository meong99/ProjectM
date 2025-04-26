// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PMGameplayAbility.h"
#include "MAbility_DefaultAttackBase.generated.h"

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
	void NotifyMontageCanceledCallBack();
/*
* Member Variables
*/
protected:
};