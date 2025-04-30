// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MTradeComponentBase.h"
#include "MPlayerTradeComponent.generated.h"

/**
 *
 */
UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class PROJECTM_API UMPlayerTradeComponent : public UMTradeComponentBase
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMPlayerTradeComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual const FMTradeResponse OnRequestSimpleDataGrant(const FMTradeRequest& Request) override;
	virtual const FMTradeResponse OnRequestSimpleTrading(const FMTradeRequest& Request) override;

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
