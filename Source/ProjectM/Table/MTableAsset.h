#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Item/MTable_ItemBase.h"
#include "MTableAsset.generated.h"

UENUM(BlueprintType)
enum class EMTableType : uint8
{
	None		UMETA(DisplayName = "None"),
	Equipment	UMETA(DisplayName="Equipment"),
	Consumable	UMETA(DisplayName = "Consumable"),
};

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
	EMTableType TableType = EMTableType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UDataTable*> Tables;
};