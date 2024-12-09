// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
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

protected:

/*
* Member Functions
*/
public:
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

/*
* Member Variables
*/
private:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpenHandles;
};