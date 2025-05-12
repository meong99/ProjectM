#include "MQuestProgressWidget.h"
#include "Definitions/MQuestDefinition.h"
#include "MQuestSlotWidget.h"
#include "Components/VerticalBox.h"
#include "Components/MPlayerQuestComponent.h"
#include "GameFramework/PlayerController.h"
#include "MQuestInfoWidget.h"

UMQuestProgressWidget::UMQuestProgressWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMQuestProgressWidget::PreAddToLayer()
{
	Super::PreAddToLayer();
}

void UMQuestProgressWidget::InitQuest(UMQuestInfoWidget* InQuestInfo)
{
	if (!InQuestInfo)
	{
		ensure(false);
		MCHAE_ERROR("PlayerQuestInfo is not valid");
		return;
	}

	APlayerController* Controller = GetOwningPlayer<APlayerController>();
	if (Controller)
	{
		PlayerQuestComponent = Controller->FindComponentByClass<UMPlayerQuestComponent>();
		if (!PlayerQuestComponent)
		{
			MCHAE_ERROR("QuestComponent is not valid");
			ensure(false);
			return;
		}
	}

	ClearQuestProgress();

	const TMap<int32, TObjectPtr<UMQuestDefinition>>& QuestDatas = PlayerQuestComponent->GetQuestDatas();
	SetInProgressQuests(QuestDatas, InQuestInfo);
	SetStartableQuests(QuestDatas, InQuestInfo);

	// 	const TSet<int32>& FinishedQuests = PlayerQuestComponent->GetFinishedQuests();
	// 	for (const int32 QuestRowId : FinishedQuests)
	// 	{
	// 		UMQuestDefinition* QuestDef = QuestDatas.FindRef(QuestRowId);
	// 	}
}

void UMQuestProgressWidget::SetInProgressQuests(const TMap<int32, TObjectPtr<UMQuestDefinition>>& QuestDatas, UMQuestInfoWidget* InQuestInfo)
{
	const TSet<int32>& InProgressingQuests = PlayerQuestComponent->GetInProgressingQuests();

	for (const int32 QuestRowId : InProgressingQuests)
	{
		UMQuestDefinition* QuestDef = QuestDatas.FindRef(QuestRowId);
		if (QuestDef)
		{
			UMQuestSlotWidget* QuestSlot = CreateWidget<UMQuestSlotWidget>(GetOwningPlayer(), QuestSlotClass);
			if (QuestSlot)
			{
				QuestSlot->InitQuestSlot(InQuestInfo, QuestDef, EMQuestState::InProgress);
				ProgressVertical->AddChildToVerticalBox(QuestSlot);
			}
		}
	}
}

void UMQuestProgressWidget::SetStartableQuests(const TMap<int32, TObjectPtr<UMQuestDefinition>>& QuestDatas, UMQuestInfoWidget* InQuestInfo)
{
	const TSet<int32>& StartableQuests = PlayerQuestComponent->GetStartableQuests();

	for (const int32 QuestRowId : StartableQuests)
	{
		UMQuestDefinition* QuestDef = QuestDatas.FindRef(QuestRowId);
		if (QuestDef)
		{
			UMQuestSlotWidget* QuestSlot = CreateWidget<UMQuestSlotWidget>(GetOwningPlayer(), QuestSlotClass);
			if (QuestSlot)
			{
				QuestSlot->InitQuestSlot(InQuestInfo, QuestDef, EMQuestState::Startable);
				StartableVertical->AddChildToVerticalBox(QuestSlot);
			}
		}
	}
}

void UMQuestProgressWidget::ClearQuestProgress()
{
	ProgressVertical->ClearChildren();
	StartableVertical->ClearChildren();
}
