// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpecHandle.h"
#include "ActiveGameplayEffectHandle.h"
#include "Templates/SubclassOf.h"
#include "MAbilityTypes.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UPMGameplayAbility;
class UPMAbilitySystemComponent;

USTRUCT(BlueprintType, Blueprintable)
struct FMAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EffectClass;
};

// Ability와 Tag를 Wrapping해놓은 구조체
USTRUCT(BlueprintType)
struct FPMAbilitySet_GameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPMGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
};

USTRUCT(BlueprintType)
struct FMAbilitySet_GrantedHandles 
{
	GENERATED_BODY()

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);

	// 부여된 어빌리티 모두 제거
	void TakeFromAbilitySystem(UPMAbilitySystemComponent* AbilitySystemComp);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};

USTRUCT(BlueprintType)
struct FPMAbilitySet_AppliedEffectHandles 
{
	GENERATED_BODY()

	void AddAppliedEffectHandle(const FActiveGameplayEffectHandle& Handle);

	void RemoveAppliedEffects(UPMAbilitySystemComponent* AbilitySystemComp);

protected:
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> AppliedEffectHandles;
};
