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

void UMPlayerQuestInfoWidget::DisplayQuestInfo(const UMQuestDefinition* QuestDefinition) const
{
	QuestInfo->DisplayQuestInfo(QuestDefinition);
}

void UMPlayerQuestInfoWidget::InitQuest()
{
	QuestProgress->InitQuest(this);
}
