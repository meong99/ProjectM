#include "MConsumableItemInstance.h"
#include "PMGameplayTags.h"
#include "MConsumableItemDefinition.h"
#include "Player/PMPlayerControllerBase.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"

UMConsumableItemInstance::UMConsumableItemInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

int32 UMConsumableItemInstance::ActivateItem()
{
	if (CanUseItem() == false)
	{
		return GetStatTagStackCount(FPMGameplayTags::Get().Item_Quentity);
	}

	const UMConsumableItemDefinition* ItemDefCDO = GetDefault<UMConsumableItemDefinition>(ItemDef);
	UPMAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ItemDefCDO && ASC)
	{
		for (const FMApplyEffectDefinition& EffectDef : ItemDefCDO->ApplyEffectToSelf)
		{
			if (EffectDef.EffectClass)
			{
				ASC->ApplyGameplayEffectToSelf(EffectDef.EffectClass->GetDefaultObject<UGameplayEffect>(), EffectDef.EffectLevel, FGameplayEffectContextHandle{});
			}
			else
			{
				MCHAE_ERROR("Effect class is not defined! check item definition! Definition name is %s", *ItemDefCDO->GetName());
			}
		}

		MCHAE_TEST("ItemUse");
		
	}

	return Super::ActivateItem();
}

bool UMConsumableItemInstance::CanUseItem() const
{
	bool bCanUseItem = false;

	if (GetStatTagStackCount(FPMGameplayTags::Get().Item_Quentity))
	{
		bCanUseItem = true;
	}
	if (GetAbilitySystemComponent())
	{
		bCanUseItem &= true;
	}

	return bCanUseItem;
}

UPMAbilitySystemComponent* UMConsumableItemInstance::GetAbilitySystemComponent() const
{
	APMPlayerControllerBase* Controller = Cast<APMPlayerControllerBase>(GetOuter());
	if (Controller)
	{
		return Controller->GetAbilitySystemComponent();
	}

	return nullptr;
}
