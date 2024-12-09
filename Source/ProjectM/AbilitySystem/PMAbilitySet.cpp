#include "PMAbilitySet.h"
#include "PMAbilitySystemComponent.h"
#include "Abilities/PMGameplayAbilitiy.h"

/*
* FPMAbilitySet_GrantedHandles --------------------------------------
*/
void FPMAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FPMAbilitySet_GrantedHandles::TakeFromAbilitySystem(UPMAbilitySystemComponent* AbilitySystemComp)
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

/*
* UPMAbilitySet --------------------------------------
*/
UPMAbilitySet::UPMAbilitySet()
{
}

void UPMAbilitySet::GiveToAbilitySystem(UPMAbilitySystemComponent* AbilitySystemComp, OUT FPMAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(AbilitySystemComp);

	if (!AbilitySystemComp->IsOwnerActorAuthoritative())
	{
		return;
	}

	// 부여해야할 모든 어빌리티를 돌면서 확인
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		// 어빌리티가 지정되어있는지 확인. 에디터에서 null로 넣어놓을 수 있기 때문.
		const FPMAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
		if (!IsValid(AbilityToGrant.Ability))
		{
			continue;
		}

		UPMGameplayAbilitiy* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UPMGameplayAbilitiy>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystemComp->GiveAbility(AbilitySpec);
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}
}
