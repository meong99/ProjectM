#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "PMInventoryItemDefinition.h"
#include "System/PMGameplayTagStack.h"

#include "PMInventoryItemInstance.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class PROJECTM_API UPMInventoryItemInstance : public UObject
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool IsSupportedForNetworking() const override { return true; }
/*
* Member Functions
*/
public:
	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (DeterminesOutputType = FragmentClass))
	const UPMInventoryItemFragment* FindFragmentByClass(TSubclassOf<UPMInventoryItemFragment> FragmentClass) const;

	template<typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

	/** add/remove stack count to stat tag(=gameplay-tag stack) */
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	/** whether stat tag has in StatTags */
	bool HasStatTag(FGameplayTag Tag) const;

	/** get the current count of gameplay-tag stack */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;
/*
* Member Variables
*/
public:
	UPROPERTY(Replicated)
	TSubclassOf<UPMInventoryItemDefinition> ItemDef;

	/** gameplay-tag stacks for inventory item instance */
	UPROPERTY(Replicated)
	FPMGameplayTagStackContainer StatTags;
};