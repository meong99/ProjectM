#include "MPlayerQuestComponent.h"
#include "Util/MGameplayStatics.h"
#include "UI/MViewportClient.h"
#include "PMGameplayTags.h"
#include "UI/MPlayerQuestInfoWidget.h"

UMPlayerQuestComponent::UMPlayerQuestComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMPlayerQuestComponent::InitializeComponent()
{
	Super::InitializeComponent();

#pragma TODO("세이브데이터 추가후 퀘스트 진행상황 로드")
	for (const auto& Iter : QuestDatas)
	{
		StartableQuests.Add(Iter.Key);
	}
}

void UMPlayerQuestComponent::AcceptQuest(const int32 QuestRowId)
{
	int32 RemovedNum = StartableQuests.Remove(QuestRowId);
	if (RemovedNum && InProgressingQuests.Contains(QuestRowId) == false)
	{
		InProgressingQuests.Add(QuestRowId);
		UpdateQuestWidget();
	}
}

void UMPlayerQuestComponent::UpdateQuestWidget() const
{
	UMViewportClient* ViewportClient = UMGameplayStatics::GetViewportClient(this);
	if (ViewportClient)
	{
		UMPlayerQuestInfoWidget* PlayerQuestInfo = Cast<UMPlayerQuestInfoWidget>(ViewportClient->GetWidgetInstance(FPMGameplayTags::Get().UI_Registry_InputTag_PlayerQuest));
		if (PlayerQuestInfo)
		{
			PlayerQuestInfo->InitQuest();
		}
	}
}
