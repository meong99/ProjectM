#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MTable_ItemBase.generated.h"

class UTexture;

/**
 *
 */
USTRUCT(BlueprintType, Blueprintable)
struct FMTable_ItemBase : public FTableRowBase
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
	int32 ItemId = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture* ItemTexture;
};