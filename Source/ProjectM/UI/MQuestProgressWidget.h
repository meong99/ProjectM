// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MQuestProgressWidget.generated.h"

class UMQuestSlotWidget;
class UVerticalBox;
class UMQuestDefinition;
class UMPlayerQuestComponent;
class UMPlayerQuestInfoWidget;

UCLASS()
class PROJECTM_API UMQuestProgressWidget : public UMWidgetBase
{
	GENERATED_BODY()

	/*
	* Overrided Functions
	*/
public:
	UMQuestProgressWidget(const FObjectInitializer& ObjectInitializer);
	virtual void PreAddToLayer() override;

	/*
	* Member Functions
	*/
public:
	void InitQuest(UMPlayerQuestInfoWidget* InPlayerQuestInfo);
	void SetInProgressQuests(const TMap<int32, TObjectPtr<UMQuestDefinition>>& QuestDatas, UMPlayerQuestInfoWidget* InPlayerQuestInfo);
	void SetStartableQuests(const TMap<int32, TObjectPtr<UMQuestDefinition>>& QuestDatas, UMPlayerQuestInfoWidget* InPlayerQuestInfo);

protected:
	/*
	* Member Variables
	*/
protected:
	UPROPERTY(BlueprintReadOnly, Category="ProjectM")
	TObjectPtr<UMPlayerQuestComponent> PlayerQuestComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectM")
	TSubclassOf<UMQuestSlotWidget> QuestSlotClass;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UVerticalBox> ProgressVertical;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UVerticalBox> StartableVertical;
};
