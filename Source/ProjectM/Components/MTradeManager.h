// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "MTradeManager.generated.h"

USTRUCT(BlueprintType)
struct FMTradeRequest
{
	GENERATED_BODY()

	enum EMRequestType
	{
		None,
		Trade,
		Give,
		Take,
	};

	int32 GetTradeId() const
	{
		static int32 Id = 0;

		return Id++;
	}

	int32 TradeId = INDEX_NONE;

	EMRequestType RequestType = None;

	// 상대에게 받을 골드 혹은 교환할 골드
	int64 RequiredGold = 0;

	// 상대에게 받을 아이템 혹은 교환할 아이템
	TMap<int32/*ItemRowId*/, int32/*Quentity*/> RequiredItems;

	// 상대에게 줄 골드 혹은 교환할 골드
	int64 GrantGold = 0;

	// 상대에게 줄 아이템 혹은 교환할 아이템
	TArray<int32> GrantItems;

	// 기타 설명용 문자열
	FString ContextString;
};

USTRUCT(BlueprintType)
struct FMTradeResponse
{
	GENERATED_BODY()

	enum EMResponseType
	{
		None,
		Error,
		Fail,
		Progress,
		Success,
	};

	int32 TradeId = INDEX_NONE;

	EMResponseType ResponseType = None;

	// 기타 설명용 문자열
	FString ContextString;
};

/**
 *
 */
UCLASS(BlueprintType)
class PROJECTM_API UMTradeManager : public UGameStateComponent
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMTradeManager(const FObjectInitializer& ObjectInitializer);

/*
* Member Functions
*/
public:
	// 검증 없이 단순히 대상자에게 데이터를 전달(몬스터 아이템 부여)
	const FMTradeResponse SimpleRequestDataGrant(AActor* Responder, const FMTradeRequest& Request);

	// 검증 없이 단순히 대상자에게 데이터를 전달 후 대상자가 데이터 검증 후 응답 (상점, 퀘스트 등)
	const FMTradeResponse SimpleRequestTrading(AActor* Requestor, AActor* Responder, const FMTradeRequest& Request);
	
protected:

/*
* Member Variables
*/
protected:
	
};
