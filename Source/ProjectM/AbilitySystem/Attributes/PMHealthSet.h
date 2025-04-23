// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PMAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PMHealthSet.generated.h"

class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamaged, AActor*, Instigator);

/**
 * 
 */
UCLASS()
class PROJECTM_API UPMHealthSet : public UPMAttributeSet
{
	GENERATED_BODY()
public:
	UPMHealthSet();

	ATTRIBUTE_ACCESSORS(UPMHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UPMHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UPMHealthSet, Healing);

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintAssignable, Category = "ProjectM | Health")
	FOnDamaged Delegate_OnDamaged;

	UPROPERTY(BlueprintReadWrite, Category = "ProjectM | Health")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadWrite, Category = "ProjectM | Health")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadWrite, Category = "ProjectM | Health")
	FGameplayAttributeData Healing;
};
