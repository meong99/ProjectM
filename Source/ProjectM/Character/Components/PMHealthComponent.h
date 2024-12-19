// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameFrameworkComponent.h"
#include "Delegates/Delegate.h"
#include "PMHealthComponent.generated.h"

class UPMAbilitySystemComponent;
class UPMHealthSet;
class UPMHealthComponent;
class AActor;
struct FOnAttributeChangeData;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FPMHealth_AttributeChanged, UPMHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);


UCLASS()
class PROJECTM_API UPMHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()
public:
	UPMHealthComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "ProjectM | Health")
	static UPMHealthComponent* FindHealthComponent(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "ProjectM | Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "ProjectM | Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "ProjectM | Health")
	float GetHealthNormalized() const;

	void InitializeWithAbilitySystem(UPMAbilitySystemComponent* InASC);
	void UninitializeWithAbilitySystem();

	void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);
	void HandleHealthChanged_Impl(UPMHealthComponent* HealthComponent, float OldValue, float NewValue, AActor* Instigator);

public:
	UPROPERTY(BlueprintAssignable)
	FPMHealth_AttributeChanged OnHealthChanged;

private:
	UPROPERTY()
	TObjectPtr<UPMAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<const UPMHealthSet> HealthSet;
};
