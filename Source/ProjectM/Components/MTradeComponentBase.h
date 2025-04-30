// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MTradeManager.h"
#include "MTradeComponentBase.generated.h"

/**
 *
 */
UCLASS(Abstract, BlueprintType, meta = (BlueprintSpawnableComponent))
class PROJECTM_API UMTradeComponentBase : public UActorComponent
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMTradeComponentBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

/*
* Member Functions
*/
public:
	virtual const FMTradeResponse OnRequestSimpleTrading(const FMTradeRequest& Request) { return {}; }
	virtual const FMTradeResponse OnRequestSimpleDataGrant(const FMTradeRequest& Request) { return {}; }
	virtual void OnReceivedResponse(const FMTradeRequest& Request, const FMTradeResponse& Response) {}

	// Do Nothing. Have to overriding
	UFUNCTION(Server, Reliable)
	virtual void Server_SendSimpleTradingRequest(const FMTradeRequest& Request, AActor* Responder);

	// Do Nothing. Have to overriding
	UFUNCTION(Server, Reliable)
	virtual void Server_SendSimpleDataGrantRequest(const FMTradeRequest& Request, AActor* Responder);
	
protected:
	const FMTradeResponse MakeErrorResponse(const FMTradeRequest& Request, const FString& ContextString = TEXT(""));
	const FMTradeResponse MakeFailResponse(const FMTradeRequest& Request, const FString& ContextString = TEXT(""));
	const FMTradeResponse MakeSuccessResponse(const FMTradeRequest& Request, const FString& ContextString = TEXT(""));

/*
* Member Variables
*/
protected:
	
};
