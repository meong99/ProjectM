#include "MPlayerQuestInfoWidget.h"
#include "MQuestProgressWidget.h"
#include "MQuestInfoWidget.h"

UMPlayerQuestInfoWidget::UMPlayerQuestInfoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMPlayerQuestInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMPlayerQuestInfoWidget::PreAddToLayer()
{
	Super::PreAddToLayer();
	InitQuest();
}

void UMPlayerQuestInfoWidget::DisplayQuestInfo(const UMQuestDefinition* QuestDefinition, const FMQuestHandle& QuestHandle) const
{
	QuestInfo->DisplayQuestInfo(QuestDefinition, QuestHandle);
}

void UMPlayerQuestInfoWidget::InitQuest()
{
	QuestProgress->InitQuest(QuestInfo);
	QuestInfo->SetVisibility(ESlateVisibility::Collapsed);
}
