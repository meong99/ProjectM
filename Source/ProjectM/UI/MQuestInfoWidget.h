// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "Definitions/MQuestDefinition.h"
#include "MQuestInfoWidget.generated.h"

class UTextBlock;
class UMQuestDefinition;
class UHorizontalBox;
class UMItemWithTextWidget;
class UPMInventoryItemDefinition;

UCLASS()
class PROJECTM_API UMQuestInfoWidget : public UMWidgetBase
{
	GENERATED_BODY()

	/*
	* Overrided Functions
	*/
public:
	UMQuestInfoWidget(const FObjectInitializer& ObjectInitializer);

	/*
	* Member Functions
	*/
public:
	void DisplayQuestInfo(const UMQuestDefinition* QuestDefinition);

protected:
	void SetRequiredItem(const TArray<FMQuestItem>& RequiredItems);
	void SetRewardItem(const TArray<FMQuestItem>& RewardItems);

	UPMInventoryItemDefinition*	GetItemDef(const int32 RowId);
	/*
	* Member Variables
	*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	TSubclassOf<UMItemWithTextWidget> ItemContextClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UTextBlock> QuestName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UTextBlock> QuestGoalContext;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UTextBlock> QuestContent;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UHorizontalBox> RequiredItemBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UHorizontalBox> RewardItemBox;
};
