#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MTable_Shop.generated.h"

/**
 *
 */
USTRUCT(BlueprintType, Blueprintable)
struct FMTable_Shop : public FTableRowBase
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
	UPROPERTY(EditDefaultsOnly)
	int32 ItemTableId = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly)
	TArray<int32> ItemIds;
};