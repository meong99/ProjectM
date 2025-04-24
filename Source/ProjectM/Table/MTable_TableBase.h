#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MTable_TableBase.generated.h"

/**
 *
 */
USTRUCT(BlueprintType, Blueprintable)
struct FMTable_TableBase : public FTableRowBase
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
	bool	IsValidId() const;
	int32	GetKey() const;
	int32	GetElementId() const;
/*
* Member Variables
*/
public:
	// x...xyyyyyy
	// x = TableId(1~nnn...), y = ElementId(00001~99999)
	UPROPERTY(EditDefaultsOnly)
	int32 RowId = INDEX_NONE;
};