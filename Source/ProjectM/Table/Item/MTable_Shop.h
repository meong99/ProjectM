#pragma once

#include "CoreMinimal.h"
#include "Table/MTable_TableBase.h"
#include "MTable_Shop.generated.h"

/**
 *
 */
USTRUCT(BlueprintType, Blueprintable)
struct FMTable_Shop : public FMTable_TableBase
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