// Fill out your copyright notice in the Description page of Project Settings.


#include "PMHealthComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/PMHealthSet.h"

UPMHealthComponent::UPMHealthComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
	SetIsReplicatedByDefault(true);
}

void UPMHealthComponent::InitializeWithAbilitySystem(UPMAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		MCHAE_ERROR("PMHealthComponent: Health component for owner [%s] has already been initialized with an ability system.", *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent = InASC;
	if (!AbilitySystemComponent)
	{
		MCHAE_ERROR("PMHealthComponent: Cannot initialize health component for owner [%s] with NULL ability system.", *GetNameSafe(Owner));
		return;
	}

	HealthSet = AbilitySystemComponent->GetSet<UPMHealthSet>();
	if (!HealthSet)
	{
		MCHAE_ERROR("PMHealthComponent: Cannot initialize health component for owner [%s] with NULL health set on the ability system.", *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPMHealthSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);

#pragma TODO("Pawn이 재생성되기때문에 Ability, AttributeSet초기화와 위젯 초기화 타이밍 등이 맞지 않는다. 새로운 함수로 초기화 해야한다.")
	// 초기화
	Multicast_HandleHealthChanged(this, 0, HealthSet->GetHealth(), nullptr);
}

void UPMHealthComponent::UninitializeWithAbilitySystem()
{
	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
}

float UPMHealthComponent::GetCurrentHealth()
{
	if (HealthSet)
	{
		return HealthSet->GetHealth();
	}
	
	MCHAE_WARNING("Health is not initialized!");
	return 0;
}

static AActor* GetInstigatorFromAttrChangeData(const FOnAttributeChangeData& ChangeData)
{
	if (ChangeData.GEModData != nullptr)
	{
		const FGameplayEffectContextHandle& EffectContext = ChangeData.GEModData->EffectSpec.GetEffectContext();
		return EffectContext.GetOriginalInstigator();
	}
	return nullptr;
}

void UPMHealthComponent::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	Multicast_HandleHealthChanged(this, ChangeData.OldValue, ChangeData.NewValue, GetInstigatorFromAttrChangeData(ChangeData));
}

void UPMHealthComponent::Multicast_HandleHealthChanged_Implementation(UPMHealthComponent* HealthComponent, float OldValue, float NewValue, AActor* Instigator)
{
	OnHealthChanged.Broadcast(HealthComponent, OldValue, NewValue, Instigator);
}

UPMHealthComponent* UPMHealthComponent::FindHealthComponent(const AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	UPMHealthComponent* HealthComponent = Actor->FindComponentByClass<UPMHealthComponent>();
	return HealthComponent;
}

float UPMHealthComponent::GetHealth() const
{
	return (HealthSet ? HealthSet->GetHealth() : 0.0f);
}

float UPMHealthComponent::GetMaxHealth() const
{
	return (HealthSet ? HealthSet->GetMaxHealth() : 0.0f);
}

float UPMHealthComponent::GetHealthNormalized() const
{
	if (HealthSet)
	{
		const float Health = HealthSet->GetHealth();
		const float MaxHealth = HealthSet->GetMaxHealth();
		return ((MaxHealth > 0.0f) ? (Health / MaxHealth) : 0.0f);
	}
	return 0.0f;
}
