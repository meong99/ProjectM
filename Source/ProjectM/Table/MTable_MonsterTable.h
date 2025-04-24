#pragma once

#include "CoreMinimal.h"
#include "MTable_TableBase.h"
#include "Templates/SubclassOf.h"
#include "MTable_MonsterTable.generated.h"

class UMMonsterDefinition;

/**
 *
 */
USTRUCT(BlueprintType, Blueprintable)
struct FMTable_MonsterTable : public FMTable_TableBase
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
	TSubclassOf<UMMonsterDefinition> MonsterDefinition;
};