#include "MQuestInfoWidget.h"
#include "Definitions/MQuestDefinition.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "MItemWithTextWidget.h"
#include "Inventory/PMInventoryItemDefinition.h"
#include "Util/MGameplayStatics.h"
#include "System/MDataTableManager.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "MQuestSlotWidget.h"

UMQuestInfoWidget::UMQuestInfoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMQuestInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMQuestInfoWidget::DisplayQuestInfo(const UMQuestDefinition* QuestDefinition, const FMQuestHandle& InQuestHandle)
{
	if (!QuestDefinition)
	{
		ensure(false);
		MCHAE_WARNING("QuestDefinition is not valid");
		return;
	}
	if (QuestHandle == InQuestHandle || !InQuestHandle)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	UpdateHandle(InQuestHandle);
	QuestName->SetText(QuestDefinition->QuestName);
	QuestGoalContext->SetText(QuestDefinition->QuestGoalContext);
	QuestContent->SetText(QuestDefinition->QuestContext);

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	SetRequiredItem(QuestHandle.TrackedRequiredItem);
	SetRewardItem(QuestDefinition->RewardItems);
}

void UMQuestInfoWidget::UpdateHandle(const FMQuestHandle& InQuestHandle)
{
	QuestHandle = InQuestHandle;
	UpdateFinishButton();
}

void UMQuestInfoWidget::OnClick_FinishButton() const
{
	if (QuestHandle && QuestHandle.Slot)
	{
		QuestHandle.Slot->OnClick_FinishButton();
	}
}

void UMQuestInfoWidget::SetRequiredItem(const TMap<int32, FMQuestItem>& RequiredItems)
{
	RequiredItemBox->ClearChildren();
	for (const auto& Iter : RequiredItems)
	{
		const FMQuestItem& Item = Iter.Value;
		UPMInventoryItemDefinition* ItemDef = GetItemDef(Item.ItemRowId);
		if (ItemDef)
		{
			UMItemWithTextWidget* ItemContextWidget = CreateWidget<UMItemWithTextWidget>(GetOwningPlayer(), ItemContextClass);
			if (ItemContextWidget)
			{
				ItemContextWidget->SetItemTexture(ItemDef->ItemIcon);

				FText Template = FText::FromString("{CurrentItemNum} / {GoaldItemNum}");
				FFormatNamedArguments Args;

				if (QuestHandle.QuestState == EMQuestState::InProgress)
				{
					Args.Add("CurrentItemNum", FText::AsNumber(Item.TrackedItemNum));
				}
				else
				{
					Args.Add("CurrentItemNum", FText::FromString("-"));
				}
				Args.Add("GoaldItemNum", FText::AsNumber(Item.ItemQuentity));
				FText Result = FText::Format(Template, Args);

				ItemContextWidget->SetItemContextText(Result);
				RequiredItemBox->AddChildToHorizontalBox(ItemContextWidget);
			}
		}
	}
}

void UMQuestInfoWidget::SetRewardItem(const TArray<FMQuestItem>& RewardItems)
{
	RewardItemBox->ClearChildren();
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

void UMQuestInfoWidget::UpdateFinishButton()
{
	EMQuestState CurrentState = QuestHandle.QuestState;
	if (EMQuestState::Startable <= CurrentState && CurrentState <= EMQuestState::Finished)
	{
		if (CurrentState == EMQuestState::CanFinish)
		{
			FinishButton->SetIsEnabled(true);
		}
		else
		{
			FinishButton->SetIsEnabled(false);
		}
		ButtonTextSwitcher->SetActiveWidgetIndex((int32)CurrentState - 1);
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
