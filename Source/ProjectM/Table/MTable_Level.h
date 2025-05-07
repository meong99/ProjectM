#pragma once

#include "CoreMinimal.h"
#include "Table/MTable_TableBase.h"
#include "GameplayTagContainer.h"
#include "MTable_Level.generated.h"

class UPMUserFacingExperienceDefinition;

/**
 *
 */
USTRUCT(BlueprintType, Blueprintable)
struct FMTable_Level : public FMTable_TableBase
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
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin=1600001, ClampMax= 1699999), Category = "ProjectM")
	FGameplayTag LevelTag;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectM")
	FString Ip;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectM")
	TObjectPtr<UPMUserFacingExperienceDefinition> UFED;
};