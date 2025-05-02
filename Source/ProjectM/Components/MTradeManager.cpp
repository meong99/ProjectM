#include "MTradeManager.h"
#include "MTradeComponentBase.h"

UMTradeManager::UMTradeManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const FMTradeResponse UMTradeManager::SimpleRequestDataGrant(AActor* Responder, const FMTradeRequest& Request)
{
	FMTradeResponse Response;
	if (!HasAuthority())
	{
		ensure(false);
		Response.TradeId = Request.TradeId;
		Response.ResponseType = FMTradeResponse::Fail;
		Response.ContextString = TEXT("TradeManager only working on authority! Don't call Client");
		return Response;
	}

	UMTradeComponentBase* ResponderComp = IsValid(Responder) ? Responder->FindComponentByClass<UMTradeComponentBase>() : nullptr;
	if (!IsValid(ResponderComp))
	{
		ensure(false);
		Response.TradeId = Request.TradeId;
		Response.ResponseType = FMTradeResponse::Fail;
		Response.ContextString = TEXT("Responder object is not valid");
		return Response;
	}

	Response = ResponderComp->OnRequestSimpleDataGrant(Request);
	return Response;
}

const FMTradeResponse UMTradeManager::SimpleRequestTrading(AActor* Responder, const FMTradeRequest& Request)
{
	FMTradeResponse Response;
	if (!HasAuthority())
	{
		ensure(false);
		Response.TradeId = Request.TradeId;
		Response.ResponseType = FMTradeResponse::Fail;
		Response.ContextString = TEXT("TradeManager only working on authority! Don't call Client");
		return Response;
	}

	UMTradeComponentBase* ResponderComp = IsValid(Responder) ? Responder->FindComponentByClass<UMTradeComponentBase>() : nullptr;
	if (!IsValid(ResponderComp))
	{
		ensure(false);
		Response.TradeId = Request.TradeId;
		Response.ResponseType = FMTradeResponse::Fail;
		Response.ContextString = TEXT("Trader object is not valid");
		return Response;
	}

	Response = ResponderComp->OnRequestSimpleTrading(Request);
	return Response;
}
