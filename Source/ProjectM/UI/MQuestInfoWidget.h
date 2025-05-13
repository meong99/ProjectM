// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "Definitions/MQuestDefinition.h"
#include "Types/MQuestTypes.h"
#include "MQuestInfoWidget.generated.h"

class UTextBlock;
class UMQuestDefinition;
class UHorizontalBox;
class UMItemWithTextWidget;
class UPMInventoryItemDefinition;
class UButton;
class UWidgetSwitcher;

UCLASS()
class PROJECTM_API UMQuestInfoWidget : public UMWidgetBase
{
	GENERATED_BODY()

	/*
	* Overrided Functions
	*/
public:
	UMQuestInfoWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeOnInitialized() override;

	/*
	* Member Functions
	*/
public:
	void	DisplayQuestInfo(const UMQuestDefinition* QuestDefinition, const FMQuestHandle& InQuestHandle);
	void	UpdateHandle(const FMQuestHandle& InQuestHandle);

	const FMQuestHandle& GetCurrentQuestHangle() const { return QuestHandle; }

protected:
	UFUNCTION()
	void	OnClick_FinishButton();

	void	SetRequiredItem(const TMap<int32, FMQuestItem>& RequiredItems);
	void	SetRewardItem(const TArray<FMQuestItem>& RewardItems);
	void	UpdateFinishButton();

	UPMInventoryItemDefinition*	GetItemDef(const int32 RowId);
	/*
	* Member Variables
	*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	TSubclassOf<UMItemWithTextWidget> ItemContextClass;

	UPROPERTY(BlueprintReadOnly, Category = "ProjectM")
	FMQuestHandle QuestHandle;

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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UButton> FinishButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "ProjectM")
	TObjectPtr<UWidgetSwitcher> ButtonTextSwitcher;
};
