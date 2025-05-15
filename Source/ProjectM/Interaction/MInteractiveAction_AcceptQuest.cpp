#include "MInteractiveAction_AcceptQuest.h"
#include "System/MDataTableManager.h"
#include "Table/MTable_QuestTable.h"
#include "Engine/Engine.h"
#include "Definitions/MQuestDefinition.h"
#include "Util/MGameplayStatics.h"
#include "UI/MViewportClient.h"
#include "PMGameplayTags.h"
#include "UI/Interaction/MDialogueBoxWidget.h"
#include "MInteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MPlayerQuestComponent.h"
#include "Components/MNpcQuestComponent.h"

UMInteractiveAction_AcceptQuest::UMInteractiveAction_AcceptQuest(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMInteractiveAction_AcceptQuest::InitAction(UMInteractionComponent* InInteractionComponent, AActor* InOwnerActor)
{
	Super::InitAction(InInteractionComponent, InOwnerActor);
}

void UMInteractiveAction_AcceptQuest::ActivateAction()
{
	Super::ActivateAction();

	UMViewportClient* ViewportClient = UMGameplayStatics::GetViewportClient(this);
	if (ViewportClient)
	{
		UMDialogueBoxWidget* DialogWidget = Cast<UMDialogueBoxWidget>(ViewportClient->GetWidgetInstance(FPMGameplayTags::Get().UI_Registry_Game_Dialogue));
		if (DialogWidget && QuestDefinition)
		{
			FMWidgetInfo WidgetInfo;
			WidgetInfo.WidgetInstigator = this;
			WidgetInfo.WidgetOwnerActor = OwnerActor;
			DialogWidget->SetWidgetInfo(WidgetInfo);

			DialogWidget->InitDialogueWidget(QuestDefinition->DialogueRowId);
			DialogWidget->SetCallback1([this](){OnClick_Accept();});
			DialogWidget->SetCallback2([this](){OnClick_Refuse();});
			DialogWidget->AddWidgetToLayer(WidgetInfo);
		}
	}
}

void UMInteractiveAction_AcceptQuest::DeactivateAction()
{
	Super::DeactivateAction();

	UMViewportClient* ViewportClient = UMGameplayStatics::GetViewportClient(this);
	if (ViewportClient)
	{
		ViewportClient->RemoveWidgetFromLayer(FPMGameplayTags::Get().UI_Registry_Game_Dialogue);
		bShouldActivate = false;
		InteractionComponent->ActivateAllOverlapAction();
	}
}

void UMInteractiveAction_AcceptQuest::OnClick_Accept()
{
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	UMPlayerQuestComponent* PlayerQuestComponent = Controller ? Controller->FindComponentByClass<UMPlayerQuestComponent>() : nullptr;
	UMNpcQuestComponent* NpcQuestComponent = OwnerActor ? OwnerActor->FindComponentByClass<UMNpcQuestComponent>() : nullptr;
	if (NpcQuestComponent)
	{
		NpcQuestComponent->AcceptQuest(PlayerQuestComponent, QuestRowId);
	}
	if (PlayerQuestComponent)
	{
		PlayerQuestComponent->AcceptQuest(QuestRowId);
	}

	DeactivateAction();
}

void UMInteractiveAction_AcceptQuest::OnClick_Refuse()
{
	DeactivateAction();
}

#if WITH_EDITOR
void UMInteractiveAction_AcceptQuest::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property->GetFName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UMInteractiveAction_AcceptQuest, QuestRowId))
	{
		ChangeQuestDefinition();
	}
}

void UMInteractiveAction_AcceptQuest::ChangeQuestDefinition()
{
	UMDataTableManager* TableManager = GEngine->GetEngineSubsystem<UMDataTableManager>();
	if (TableManager)
	{
		const UDataTable* QuestTable = TableManager->GetDataTable(QuestRowId);
		int32 ElementIndex = UMDataTableManager::ChangeRowIdToElementId(QuestRowId) - 1;
		const TArray<FName>& Names = QuestTable->GetRowNames();
		if (Names.IsValidIndex(ElementIndex))
		{
			FMTable_QuestTable* RowData = QuestTable->FindRow<FMTable_QuestTable>(Names[ElementIndex], Names[ElementIndex].ToString());
			if (RowData && RowData->Definition)
			{
				QuestDefinition = DuplicateObject(RowData->Definition->GetDefaultObject<UMQuestDefinition>(), this);
				ActionName = QuestDefinition->QuestName;
			}
		}
	}
}

#endif
