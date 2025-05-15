#include "MQuestDefinition.h"

UMQuestDefinition::UMQuestDefinition(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/) : Super(ObjectInitializer)
{

}

FMTradeRequest UMQuestDefinition::CreateTradeRequest() const
{
	FMTradeRequest Request = FMTradeRequest::GenerateRequest(EMRequestType::Trade);
	for (const FMQuestItem& RequiredItem : RequiredItems)
	{
		Request.RequiredItems.Add({ RequiredItem.ItemRowId, RequiredItem.ItemQuentity });
	}
	for (const FMQuestItem& RewardItem : RewardItems)
	{
		Request.GrantItems.Add({ RewardItem.ItemRowId, RewardItem.ItemQuentity });
	}
	Request.GrantGold = RewardGold;

	return Request;
}
