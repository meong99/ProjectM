// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MQuestSlotWidget.generated.h"

class UTextBlock;
class UMQuestDefinition;
class UMPlayerQuestInfoWidget;

UCLASS()
class PROJECTM_API UMQuestSlotWidget : public UMWidgetBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMQuestSlotWidget(const FObjectInitializer& ObjectInitializer);
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

/*
* Member Functions
*/
public:
	void InitQuestSlot(UMPlayerQuestInfoWidget* InPlayerQuestInfo, const UMQuestDefinition* InQuestDefinition);

protected:
	void SetQuestName(const FText& InQuestName);

	/*
* Member Variables
*/
protected:
	UPROPERTY(BlueprintReadOnly, Category="ProjectM")
	TObjectPtr<UMPlayerQuestInfoWidget> PlayerQuestInfo;
	
	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	const UMQuestDefinition* QuestDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UTextBlock> QuestName;
};
