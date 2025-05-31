#include "MConsumableItemInstance.h"
#include "PMGameplayTags.h"
#include "MConsumableItemDefinition.h"
#include "Player/PMPlayerControllerBase.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"

UMConsumableItemInstance::UMConsumableItemInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

bool UMConsumableItemInstance::ActivateItem()
{
	const bool bIsActivated = Super::ActivateItem();

	if (bIsActivated)
	{
		const UMConsumableItemDefinition* ItemDefCDO = GetDefault<UMConsumableItemDefinition>(ItemDef);
		UPMAbilitySystemComponent* ASC = GetAbilitySystemComponent();
		if (ItemDefCDO && ASC)
		{
			for (const FMApplyEffectDefinition& EffectDef : ItemDefCDO->ApplyEffectToSelf)
			{
				if (EffectDef.EffectClass)
				{
					FGameplayEffectContextHandle EffectContextHandle = ASC->MakeGameplayEffectContext(Cast<APMPlayerControllerBase>(GetOuter()), nullptr);
					EffectContextHandle.AddSourceObject(this);

					TMap<FGameplayTag, float> SetbyCallerMap;
					for (const FMSetbyCallerFloat& Value : EffectDef.EffectValues)
					{
						if (Value.SetByCallerTag.IsValid())
						{
							SetbyCallerMap.Add(Value.SetByCallerTag, Value.Value);
						}
					}

					const FGameplayEffectSpec& Spec = ASC->MakeGameplayEffectSpecWithSetByCaller(EffectContextHandle, EffectDef.EffectClass, SetbyCallerMap);

					ASC->ApplyGameplayEffectSpecToSelf(Spec);
				}
				else
				{
					MCHAE_ERROR("Effect class is not defined! check item definition! Definition name is %s", *ItemDefCDO->GetName());
				}
			}

			MCHAE_TEST("ItemUse");
		
		}
	}

	return bIsActivated;
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
