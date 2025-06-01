#include "PMHealthSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Util/MGameplayStatics.h"

UPMHealthSet::UPMHealthSet() : Super(), Health(1.f), MaxHealth(1.f)
{
}

void UPMHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPMHealthSet, Health);
	DOREPLIFETIME(UPMHealthSet, MaxHealth);
}

void UPMHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UPMHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UPMHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		CheckZeroHealth();
		AdjustAttributeForMaxChange(GetHealth(), OldValue, NewValue, GetHealthAttribute());
	}
}

void UPMHealthSet::AdjustAttributeForMaxChange(const float CurrentValue, const float OldMaxValue, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent*	AbilityComp = GetOwningAbilitySystemComponent();

	if (!FMath::IsNearlyEqual(OldMaxValue, NewMaxValue) && IsValid(AbilityComp))
	{
		float	NewDelta = (OldMaxValue > 0.f) ? (CurrentValue * NewMaxValue / OldMaxValue) - CurrentValue : NewMaxValue;
		if (!FMath::IsNearlyEqual(NewDelta, 0.f))
		{
			AbilityComp->ApplyModToAttribute(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
		}
	}
}

void UPMHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}

void UPMHealthSet::CheckZeroHealth()
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	if (FMath::IsNearlyEqual(GetHealth(), 0.f))
	{
		AbilityComp->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, GetMaxHealth());
	}
}
