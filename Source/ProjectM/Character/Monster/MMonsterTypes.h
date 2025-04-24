#pragma once

#include "MMonsterTypes.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FMMonsterInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MonsterName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MonsterHp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MonsterReward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MonsterRewardItemId;
};
