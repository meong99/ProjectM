// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Character/Monster/MMonsterTypes.h"
#include "Templates/SubclassOf.h"
#include "MDefinitionBase.h"
#include "MMonsterDefinition.generated.h"

class AMMonsterBase;

/**
 *
 */
UCLASS(Blueprintable, DefaultToInstanced, EditInlineNew)
class PROJECTM_API UMMonsterDefinition : public UMDefinitionBase
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMMonsterDefinition();

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FMMonsterInfo MonsterInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1, ClampMax = 5000))
	int32 MonsterSpawningCooldown = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1, ClampMax = 10))
	int32 MaximumSpawnNum = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AMMonsterBase> MonsterClass;
};
