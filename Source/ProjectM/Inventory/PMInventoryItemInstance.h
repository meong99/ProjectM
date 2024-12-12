#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "PMInventoryItemDefinition.h"

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
/*
* Member Variables
*/
public:
	UPROPERTY()
	TSubclassOf<UPMInventoryItemDefinition> ItemDef;
};