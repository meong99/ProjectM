// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MPlayerQuestInfoWidget.generated.h"

class UMQuestProgressWidget;
class UMQuestInfoWidget;
class UMQuestDefinition;

UCLASS()
class PROJECTM_API UMPlayerQuestInfoWidget : public UMWidgetBase
{
	GENERATED_BODY()

	/*
	* Overrided Functions
	*/
public:
	UMPlayerQuestInfoWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	virtual void PreAddToLayer() override;

/*
* Member Functions
*/
public:
	void DisplayQuestInfo(const UMQuestDefinition* QuestDefinition) const;

protected:
	void InitQuest();

/*
* Member Variables
*/
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UMQuestProgressWidget> QuestProgress;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UMQuestInfoWidget> QuestInfo;
};
