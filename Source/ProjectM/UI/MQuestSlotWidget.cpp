#include "MQuestSlotWidget.h"
#include "Definitions/MQuestDefinition.h"
#include "Components/TextBlock.h"
#include "MPlayerQuestInfoWidget.h"
#include "MQuestInfoWidget.h"
#include "Player/PMPlayerControllerBase.h"
#include "Inventory/PMInventoryManagerComponent.h"

UMQuestSlotWidget::UMQuestSlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FReply UMQuestSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (QuestInfo)
	{
		QuestInfo->DisplayQuestInfo(QuestDefinition, QuestHandle);
	}

	return FReply::Handled();
}

void UMQuestSlotWidget::InitQuestSlot(UMQuestInfoWidget* InQuestInfo, const UMQuestDefinition* InQuestDefinition, EMQuestState InQuestState)
{
	if (!InQuestDefinition || !InQuestInfo)
	{
		ensure(false);
		MCHAE_WARNING("QuestDefinition is not valid!");
		return;
	}

	QuestHandle.QuestUid = QuestHandle.GenerateQuestUid();
	QuestHandle.QuestState = InQuestState;
	QuestHandle.Slot = this;
	
	for (FMQuestItem Item : InQuestDefinition->RequiredItems)
	{
		QuestHandle.TrackedRequiredItem.Add(Item.ItemRowId, Item);
	}

	QuestDefinition = InQuestDefinition;
	QuestInfo = InQuestInfo;
	if (QuestHandle.QuestState == EMQuestState::InProgress)
	{
		StartQuestTracking();
	}
	else
	{
		FinishQuestTracking();
	}
	SetQuestName(QuestDefinition->QuestName);
}

void UMQuestSlotWidget::OnClick_FinishButton()
{
	QuestHandle.QuestState = EMQuestState::Finished;
	FinishQuestTracking();
}

void UMQuestSlotWidget::SetQuestName(const FText& InQuestName)
{
	QuestName->SetText(InQuestName);
}

void UMQuestSlotWidget::StartQuestTracking()
{
#pragma TODO("이거 최적화 해야함")
	APMPlayerControllerBase* Controller = GetPlayerController();
	UPMInventoryManagerComponent* InvenManager = Controller ? Controller->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
	if (InvenManager)
	{
		Handle_AddItem = InvenManager->Delegate_OnItemIncreased.AddLambda([this](const FMItemResponse& ItemRespons)->void
		{
			FMQuestItem* QuestItem = QuestHandle.TrackedRequiredItem.Find(ItemRespons.ItemRequest.ItemRowId);
			if (QuestItem)
			{
				QuestItem->TrackedItemNum = ItemRespons.ItemQuentity;
			}

			bool bIsSuccessed = true;
			for (const auto& Iter : QuestHandle.TrackedRequiredItem)
			{
				const FMQuestItem& Value = Iter.Value;
				if (Value.ItemQuentity > Value.TrackedItemNum)
				{
					bIsSuccessed = false;
					break;
				}
			}
			if (bIsSuccessed)
			{
				QuestHandle.QuestState = EMQuestState::CanFinish;
			}
			UpdateQuestInfo();
		});

		Handle_RemoveItem = InvenManager->Delegate_OnItemDecreased.AddLambda([this](const FMItemResponse& ItemRespons)->void
		{
			FMQuestItem* QuestItem = QuestHandle.TrackedRequiredItem.Find(ItemRespons.ItemRequest.ItemRowId);
			if (QuestItem)
			{
				QuestItem->TrackedItemNum = ItemRespons.ItemQuentity;

				if (QuestItem->TrackedItemNum < QuestItem->ItemQuentity)
				{
					QuestHandle.QuestState = EMQuestState::InProgress;
				}
			}
			UpdateQuestInfo();
		});

		for (const FMQuestItem QuestItem : QuestDefinition->RequiredItems)
		{
			int32 Quentity = InvenManager->GetItemQuantity(QuestItem.ItemRowId);
			FMQuestItem* Value = QuestHandle.TrackedRequiredItem.Find(QuestItem.ItemRowId);
			if (Value)
			{
				Value->TrackedItemNum = Quentity;
			}
		}
	}
	UpdateQuestInfo();
}

void UMQuestSlotWidget::FinishQuestTracking()
{
	APMPlayerControllerBase* Controller = GetPlayerController();
	UPMInventoryManagerComponent* InvenManager = Controller ? Controller->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
	if (InvenManager)
	{
		InvenManager->Delegate_OnItemIncreased.Remove(Handle_AddItem);
		InvenManager->Delegate_OnItemDecreased.Remove(Handle_RemoveItem);
	}
	UpdateQuestInfo();
}

void UMQuestSlotWidget::UpdateQuestInfo()
{
	if (QuestInfo && QuestInfo->GetCurrentQuestHangle() == QuestHandle)
	{
		QuestInfo->UpdateHandle(QuestHandle);
	}
}
