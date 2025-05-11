#include "MInteractiveAction_AcceptQuest.h"
#include "System/MDataTableManager.h"
#include "Table/MTable_QuestTable.h"
#include "Engine/Engine.h"
#include "Definitions/MQuestDefinition.h"

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
}

void UMInteractiveAction_AcceptQuest::DeactivateAction()
{
	Super::DeactivateAction();
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
