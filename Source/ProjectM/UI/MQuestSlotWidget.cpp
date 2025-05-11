#include "MQuestSlotWidget.h"
#include "Definitions/MQuestDefinition.h"
#include "Components/TextBlock.h"
#include "MPlayerQuestInfoWidget.h"

UMQuestSlotWidget::UMQuestSlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FReply UMQuestSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	PlayerQuestInfo->DisplayQuestInfo(QuestDefinition);

	return FReply::Handled();
}

void UMQuestSlotWidget::InitQuestSlot(UMPlayerQuestInfoWidget* InPlayerQuestInfo, const UMQuestDefinition* InQuestDefinition)
{
	if (!InQuestDefinition || !InPlayerQuestInfo)
	{
		ensure(false);
		MCHAE_WARNING("QuestDefinition is not valid!");
		return;
	}

	QuestDefinition = InQuestDefinition;
	PlayerQuestInfo = InPlayerQuestInfo;
	SetQuestName(QuestDefinition->QuestName);
}

void UMQuestSlotWidget::SetQuestName(const FText& InQuestName)
{
	QuestName->SetText(InQuestName);
}
