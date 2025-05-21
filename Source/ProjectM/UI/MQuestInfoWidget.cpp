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
#include "Engine/Engine.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/MNavigationComponent.h"
#include "Character/NPC/MNpcDefinition.h"
#include "GameModes/PMGameStateBase.h"
#include "GameModes/MWorldSettings.h"
#include "Engine/Level.h"

UMQuestInfoWidget::UMQuestInfoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMQuestInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FinishButton->OnClicked.AddDynamic(this, &UMQuestInfoWidget::OnClick_FinishButton);
	SearchNpc->OnClicked.AddDynamic(this, &UMQuestInfoWidget::OnClick_SearchNpcButton);

	UMDataTableManager* TableManager = GEngine->GetEngineSubsystem<UMDataTableManager>();
	if (TableManager)
	{
		const UDataTable* ItemTable = TableManager->GetDataTable(EMTableKey::OthersItem);
		if (ItemTable)
		{
			TArray<FMTable_ItemBase*> ItemRows;
			ItemTable->GetAllRows<FMTable_ItemBase>(TEXT(""), ItemRows);
			if (ItemRows.Num() > 0 && ItemRows[0] && ItemRows[0]->Definition)
			{
				GoldDefinition = DuplicateObject(ItemRows[0]->Definition->GetDefaultObject<UPMInventoryItemDefinition>(), this);
			}
		}
	}
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
		QuestHandle = FMQuestHandle{};
		return;
	}

	if (bOnActivatedNavigation)
	{
		StopSearchNpc();
	}
	UpdateHandle(InQuestHandle);
	QuestName->SetText(QuestDefinition->QuestName);
	QuestGoalContext->SetText(QuestDefinition->QuestGoalContext);
	QuestContent->SetText(QuestDefinition->QuestContext);
	SetRewardItem(QuestDefinition->RewardItems);
	SetRewardGold(QuestDefinition->RewardGold);

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UMQuestInfoWidget::UpdateHandle(const FMQuestHandle& InQuestHandle)
{
	QuestHandle = InQuestHandle;
	SetRequiredItem(QuestHandle.TrackedRequiredItem);
	UpdateFinishButton();
}

void UMQuestInfoWidget::OnClick_FinishButton()
{
	if (QuestHandle && QuestHandle.Slot && bOnActivatedNavigation)
	{
		QuestHandle.Slot->OnClick_FinishButton();
		StopSearchNpc();
	}
}

void UMQuestInfoWidget::OnClick_SearchNpcButton()
{
	if (bOnActivatedNavigation)
	{
		StopSearchNpc();
	}

	APawn* OwningPlayerPawn = GetOwningPlayerPawn();
	UMNavigationComponent* NavigationComp = OwningPlayerPawn ? OwningPlayerPawn->FindComponentByClass<UMNavigationComponent>() : nullptr;
	const UMQuestDefinition* QuestDefinition = QuestHandle.Slot ? QuestHandle.Slot->GetQuestDefinition() : nullptr;
	UMNpcDefinition* OwnerNpcDefinition = QuestDefinition ? UMDataTableManager::GetDefinitionObject<UMNpcDefinition>(this, QuestDefinition->OwnerNpcRowId) : nullptr;

	ULevel* Level = OwningPlayerPawn ? OwningPlayerPawn->GetLevel() : nullptr;
	AMWorldSettings* WorldSetting = Level ? Cast<AMWorldSettings>(Level->GetWorldSettings()) : nullptr;
	if (NavigationComp && OwnerNpcDefinition && WorldSetting)
	{
		NavigationComp->ActivateNavigation(OwnerNpcDefinition->SearchTag, WorldSetting->WorldTag);
		bOnActivatedNavigation = true;
	}
}

void UMQuestInfoWidget::StopSearchNpc()
{
	APlayerController* Controller = GetOwningPlayer();
	UMNavigationComponent* NavigationComp = Controller ? Controller->FindComponentByClass<UMNavigationComponent>() : nullptr;

	if (NavigationComp)
	{
		NavigationComp->DeactivateNavigation();
		bOnActivatedNavigation = false;
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

				if (QuestHandle.QuestState == EMQuestState::InProgress || QuestHandle.QuestState == EMQuestState::CanFinish)
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
				UHorizontalBoxSlot* NewSlot = RequiredItemBox->AddChildToHorizontalBox(ItemContextWidget);
				if (NewSlot)
				{
					NewSlot->SetPadding(FMargin(5.f));
				}
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
				UHorizontalBoxSlot* NewSlot = RewardItemBox->AddChildToHorizontalBox(ItemContextWidget);
				if (NewSlot)
				{
					NewSlot->SetPadding(FMargin(5.f));
				}
			}
		}
	}
}

void UMQuestInfoWidget::SetRewardGold(int32 Gold)
{
	if (Gold > 0 && GoldDefinition)
	{
		UMItemWithTextWidget* ItemContextWidget = CreateWidget<UMItemWithTextWidget>(GetOwningPlayer(), ItemContextClass);

		if (ItemContextWidget)
		{
			ItemContextWidget->SetItemTexture(GoldDefinition->ItemIcon);
			ItemContextWidget->SetItemContextText(FText::AsNumber(Gold));
			UHorizontalBoxSlot* NewSlot = RewardItemBox->AddChildToHorizontalBox(ItemContextWidget);
			if (NewSlot)
			{
				NewSlot->SetPadding(FMargin(5.f));
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
