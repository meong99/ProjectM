// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilities/Public/ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "PMAbilitySystemComponent.generated.h"

UCLASS()
class PROJECTM_API UPMAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMAbilitySystemComponent();
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

protected:

/*
* Member Functions
*/
public:
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

	UFUNCTION(BlueprintCallable)
	FActiveGameplayEffectHandle ApplyEffectToTargetWithSetByCaller(TSubclassOf<UGameplayEffect> EffectClass, AActor* Target, AActor* EffectCauser, TMap<FGameplayTag, float> SetbyCallerMap, float Level = 0);
	UFUNCTION(BlueprintCallable)
	FActiveGameplayEffectHandle ApplyEffectToSelfWithSetByCaller(TSubclassOf<UGameplayEffect> EffectClass, AActor* EffectCauser, TMap<FGameplayTag, float> SetbyCallerMap, float Level = 0);

/*
* Member Variables
*/
private:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpenHandles;
};