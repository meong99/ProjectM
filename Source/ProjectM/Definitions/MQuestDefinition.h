#pragma once

#include "CoreMinimal.h"
#include "MDefinitionBase.h"
#include "MQuestDefinition.generated.h"

class AMNpcBase;

USTRUCT(BlueprintType)
struct FMQuestItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=())
	int32 ItemRowId = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 1))
	int32 ItemQuentity = 1;
};

UCLASS(Blueprintable, BlueprintType)
class PROJECTM_API UMQuestDefinition : public UMDefinitionBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMQuestDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(), Category="ProjectM")
	TSubclassOf<AMNpcBase> QuestOwner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(), Category="ProjectM")
	FText QuestName;

	// 퀘스트 목표
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(), Category="ProjectM")
	FText QuestGoalContext;

	// 퀘스트 내용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(), Category="ProjectM")
	FText QuestContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(), Category="ProjectM")
	TArray<FMQuestItem> RequiredItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(), Category="ProjectM")
	TArray<FMQuestItem> RewardItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0, ClampMax=2147483647), Category="ProjectM")
	int32 RewardGold = 0;
};
