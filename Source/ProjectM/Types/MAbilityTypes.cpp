#include "Types/MAbilityTypes.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"

void FMAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FMAbilitySet_GrantedHandles::TakeFromAbilitySystem(UPMAbilitySystemComponent* AbilitySystemComp)
{
	if (!AbilitySystemComp->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySystemComp->ClearAbility(Handle);
		}
	}
}

void FPMAbilitySet_AppliedEffectHandles::AddAppliedEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	AppliedEffectHandles.Add(Handle);
}

void FPMAbilitySet_AppliedEffectHandles::RemoveAppliedEffects(UPMAbilitySystemComponent* AbilitySystemComp)
{
	if (AbilitySystemComp)
	{
		for (const FActiveGameplayEffectHandle& Handle : AppliedEffectHandles)
		{
			AbilitySystemComp->RemoveActiveGameplayEffect(Handle);
		}

		AppliedEffectHandles.Empty();
	}
}

