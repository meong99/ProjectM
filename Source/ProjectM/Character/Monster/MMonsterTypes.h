#pragma once

#include "Templates/SubclassOf.h"
#include "Types/MAnimTypes.h"
#include "MMonsterTypes.generated.h"

class UBehaviorTree;
class AMMonsterBase;
class UGameplayEffect;

USTRUCT(BlueprintType, Blueprintable)
struct FMDropInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1100001, ClampMax = 1199999))
	int32 ItemId = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1))
	int32 ItemCount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0.0f, ClampMax=1.0f))
	float DropChance = 0.0f;
};

USTRUCT(BlueprintType, Blueprintable)
struct FMMonsterInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AMMonsterBase> MonsterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName MonsterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1, ClampMax = 2147483647))
	int32 MonsterHp = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0, ClampMax = 2147483647))
	int32 MonsterReward = 0;

	// Key = ItemId, Value = 확률
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FMDropInfo> ItemDropTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DefaultApplyEffect;
};

USTRUCT(BlueprintType, Blueprintable)
struct FMMonsterCombatInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMMontageInfo MontageInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageApplyEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MonsterDefaultAttackRange = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MonsterDefaultAttackRadius = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MonsterAttackPower = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MonsterDefensePower = 0.f;
};

