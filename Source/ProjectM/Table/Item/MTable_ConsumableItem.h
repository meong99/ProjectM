#pragma once

#include "CoreMinimal.h"
#include "MTable_ItemBase.h"
#include "MTable_ConsumableItem.generated.h"

/**
 *
 */
USTRUCT(BlueprintType, Blueprintable)
struct FMTable_ConsumableItem : public FMTable_ItemBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:

/*
* Member Functions
*/
public:
/*
* Member Variables
*/
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Health = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Mana = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Exp = 0;
};