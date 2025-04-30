#include "MMonsterTradeComponent.h"

UMMonsterTradeComponent::UMMonsterTradeComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const FMTradeResponse UMMonsterTradeComponent::OnRequestSimpleTrading(const FMTradeRequest& Request)
{
	return MakeSuccessResponse(Request);
}

void UMMonsterTradeComponent::Server_SendSimpleDataGrantRequest_Implementation(const FMTradeRequest& Request, AActor* Responder)
{
	UMTradeManager* TradeManager = GetWorld()->GetGameState()->FindComponentByClass<UMTradeManager>();
	if (!TradeManager)
	{
		ensure(false);
		MakeErrorResponse(Request, TEXT("Can't Found TradeManager"));
	}

	FMTradeResponse Response = TradeManager->SimpleRequestDataGrant(Responder, Request);
	OnReceivedResponse(Request, Response);
}

void UMMonsterTradeComponent::OnReceivedResponse(const FMTradeRequest& Request, const FMTradeResponse& Response)
{
	// 실패라면 그냥 아이템 떨구기?
}
