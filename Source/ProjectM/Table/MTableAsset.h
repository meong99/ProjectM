#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Item/MTable_ItemBase.h"
#include "MTableAsset.generated.h"

class UTexture;

/**
 *
 */
UCLASS()
class UMTableAsset : public UPrimaryDataAsset
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
	TArray<UDataTable*> Tables;
};