// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Character/Monster/MMonsterTypes.h"
#include "Templates/SubclassOf.h"
#include "MMonsterDefinition.generated.h"

class AMMonsterBase;

/**
 *
 */
UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInlineNew)
class PROJECTM_API UMMonsterDefinition : public UObject
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMMonsterInfo MonsterInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MonsterSpawningCooldown = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaximumSpawnNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AMMonsterBase> MonsterClass;
};
