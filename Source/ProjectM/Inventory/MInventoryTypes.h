#pragma once

#include "CoreMinimal.h"

#include "MInventoryTypes.generated.h"

UENUM(BlueprintType)
enum class EMItemType : uint8
{
	Equipment,
	Consumable,
	Others,

	None,
};

USTRUCT(BlueprintType)
struct FMItemHandle 
{
	GENERATED_BODY()

	bool IsValid() const
	{
		return ItemUid != INDEX_NONE;
	}

	UPROPERTY()
	int32 ItemUid = INDEX_NONE;

	UPROPERTY()
	EMItemType ItemType = EMItemType::None;
};
