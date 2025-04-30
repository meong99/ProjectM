#include "MTradeComponentBase.h"

UMTradeComponentBase::UMTradeComponentBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMTradeComponentBase::Server_SendSimpleTradingRequest_Implementation(const FMTradeRequest& Request, AActor* Responder)
{
	ensure(false);
	MCHAE_ERROR("Server_SendSimpleTradingRequest function in UMTradeComponentBase does nothing by default! if you want to use it, have to override it");
}

void UMTradeComponentBase::Server_SendSimpleDataGrantRequest_Implementation(const FMTradeRequest& Request, AActor* Responder)
{
	ensure(false);
	MCHAE_ERROR("Server_SendSimpleDataGrantRequest function in UMTradeComponentBase does nothing by default! if you want to use it, have to override it");
}

const FMTradeResponse UMTradeComponentBase::MakeErrorResponse(const FMTradeRequest& Request, const FString& ContextString)
{
	FMTradeResponse Response;

	Response.TradeId = Request.TradeId;
	Response.ResponseType = FMTradeResponse::Error;
	Response.ContextString = ContextString;

	return Response;
}

const FMTradeResponse UMTradeComponentBase::MakeFailResponse(const FMTradeRequest& Request, const FString& ContextString)
{
	FMTradeResponse Response;

	Response.TradeId = Request.TradeId;
	Response.ResponseType = FMTradeResponse::Fail;
	Response.ContextString = ContextString;

	return Response;
}

const FMTradeResponse UMTradeComponentBase::MakeSuccessResponse(const FMTradeRequest& Request, const FString& ContextString)
{
	FMTradeResponse Response;

	Response.TradeId = Request.TradeId;
	Response.ResponseType = FMTradeResponse::Success;
	Response.ContextString = ContextString;

	return Response;
}
