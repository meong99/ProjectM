// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MQuestComponentBase.h"
#include "MPlayerQuestComponent.generated.h"

/**
 *
 */
UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class PROJECTM_API UMPlayerQuestComponent : public UMQuestComponentBase
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMPlayerQuestComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void InitializeComponent() override;

	void AcceptQuest(const int32 QuestRowId);

/*
* Member Functions
*/
public:
	const TSet<int32>& GetInProgressingQuests() const { return InProgressingQuests; }
	const TSet<int32>& GetStartableQuests() const { return StartableQuests; }
	const TSet<int32>& GetFinishedQuests() const { return FinishedQuests; }
protected:
	void UpdateQuestWidget() const;

/*
* Member Variables
*/
protected:
	UPROPERTY(BlueprintReadWrite, Category="ProjectM")
	TSet<int32> InProgressingQuests;

	UPROPERTY(BlueprintReadWrite, Category="ProjectM")
	TSet<int32> StartableQuests;

	UPROPERTY(BlueprintReadWrite, Category="ProjectM")
	TSet<int32> FinishedQuests;
};
