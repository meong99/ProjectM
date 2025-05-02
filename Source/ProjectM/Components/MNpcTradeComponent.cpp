#include "MNpcTradeComponent.h"
#include "MTradeManager.h"

UMNpcTradeComponent::UMNpcTradeComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const FMTradeResponse UMNpcTradeComponent::OnRequestSimpleTrading(const FMTradeRequest& Request)
{
	return MakeSuccessResponse(Request);
}

void UMNpcTradeComponent::OnReceivedResponse(const FMTradeRequest& Request, const FMTradeResponse& Response)
{
	// 재구매??
}

void UMNpcTradeComponent::Server_SendSimpleTradingRequest_Implementation(const FMTradeRequest& Request, AActor* Responder)
{
	UMTradeManager* TradeManager = GetWorld()->GetGameState()->FindComponentByClass<UMTradeManager>();
	if (!TradeManager)
	{
		ensure(false);
		MakeErrorResponse(Request, TEXT("Can't Found TradeManager"));
	}

	FMTradeResponse Response = TradeManager->SimpleRequestTrading(Responder, Request);
	OnReceivedResponse(Request, Response);
}
