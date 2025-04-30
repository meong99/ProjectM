// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MTradeComponentBase.h"
#include "MNpcTradeComponent.generated.h"

/**
 *
 */
UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class PROJECTM_API UMNpcTradeComponent : public UMTradeComponentBase
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMNpcTradeComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual const FMTradeResponse OnRequestSimpleTrading(const FMTradeRequest& Request) override;
	virtual void Server_SendSimpleTradingRequest_Implementation(const FMTradeRequest& Request, AActor* Responder) override;
	virtual void OnReceivedResponse(const FMTradeRequest& Request, const FMTradeResponse& Response) override;

	/*
* Member Functions
*/
public:

protected:

/*
* Member Variables
*/
protected:
	
};
