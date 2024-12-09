// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PMGameplayAbilitiy.generated.h"

UENUM(BlueprintType)
enum class EPMAbilityActivationPolicy : uint8
{
	// Input이 Trigger되었을 때(Preddes/Released)
	OnInputTriggered,
	// Input이 Held되었을 때
	WhileInputActive,
	//Avatar가 생성되었을 때 즉시 적용
	OnSpawn,
};

UCLASS()
class PROJECTM_API UPMGameplayAbilitiy : public UGameplayAbility
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMGameplayAbilitiy();

protected:

/*
* Member Functions
*/
public:
	EPMAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM | AbilityActivation")
	EPMAbilityActivationPolicy ActivationPolicy;
};