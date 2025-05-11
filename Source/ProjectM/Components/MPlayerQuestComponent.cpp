#include "MPlayerQuestComponent.h"

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
