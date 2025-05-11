#include "MQuestInfoWidget.h"
#include "Definitions/MQuestDefinition.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "MItemWithTextWidget.h"
#include "Inventory/PMInventoryItemDefinition.h"
#include "Util/MGameplayStatics.h"
#include "System/MDataTableManager.h"

UMQuestInfoWidget::UMQuestInfoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMQuestInfoWidget::DisplayQuestInfo(const UMQuestDefinition* QuestDefinition)
{
	if (!QuestDefinition)
	{
		ensure(false);
		MCHAE_WARNING("QuestDefinition is not valid");
		return;
	}

	QuestName->SetText(QuestDefinition->QuestName);
	QuestGoalContext->SetText(QuestDefinition->QuestGoalContext);
	QuestContent->SetText(QuestDefinition->QuestContext);

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SetRequiredItem(QuestDefinition->RequiredItems);
	SetRewardItem(QuestDefinition->RewardItems);
}

void UMQuestInfoWidget::SetRequiredItem(const TArray<FMQuestItem>& RequiredItems)
{
	for (const FMQuestItem& Item : RequiredItems)
	{
		UPMInventoryItemDefinition* ItemDef = GetItemDef(Item.ItemRowId);
		if (ItemDef)
		{
			UMItemWithTextWidget* ItemContextWidget = CreateWidget<UMItemWithTextWidget>(GetOwningPlayer(), ItemContextClass);
			if (ItemContextWidget)
			{
				ItemContextWidget->SetItemTexture(ItemDef->ItemIcon);
				ItemContextWidget->SetItemContextText(FText::AsNumber(Item.ItemQuentity));
				RequiredItemBox->AddChildToHorizontalBox(ItemContextWidget);
			}
		}
	}
}

void UMQuestInfoWidget::SetRewardItem(const TArray<FMQuestItem>& RewardItems)
{
	for (const FMQuestItem& Item : RewardItems)
	{
		UPMInventoryItemDefinition* ItemDef = GetItemDef(Item.ItemRowId);
		if (ItemDef)
		{
			UMItemWithTextWidget* ItemContextWidget = CreateWidget<UMItemWithTextWidget>(GetOwningPlayer(), ItemContextClass);
			if (ItemContextWidget)
			{
				ItemContextWidget->SetItemTexture(ItemDef->ItemIcon);
				ItemContextWidget->SetItemContextText(FText::AsNumber(Item.ItemQuentity));
				RewardItemBox->AddChildToHorizontalBox(ItemContextWidget);
			}
		}
	}
}

UPMInventoryItemDefinition* UMQuestInfoWidget::GetItemDef(const int32 RowId)
{
	UMDataTableManager* TableManager = UMGameplayStatics::GetDataTableManager();
	if (!TableManager)
	{
		ensure(false);
		MCHAE_WARNING("TableManager is not valid");
		return nullptr;
	}

	return TableManager->GetItemDefinition(RowId);
}
