// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "MTradeManager.generated.h"

UENUM(BlueprintType)
enum class EMRequestType : uint8
{
	None,
	Trade,
	Give,
	Take,
};

USTRUCT(BlueprintType)
struct FMTradeItemInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<int32> ItemRowIds;

	UPROPERTY()
	TArray<int32> ItemCount;
};

USTRUCT(BlueprintType)
struct FMTradeRequest
{
	GENERATED_BODY()

	FMTradeRequest()
	{
		TradeId = GetTradeId();
	}

	int32 GetTradeId() const
	{
		static int32 Id = 0;
		if (TradeId != INDEX_NONE)
		{
			return TradeId;
		}

		return Id++;
	}

	UPROPERTY()
	int32 TradeId = INDEX_NONE;

	UPROPERTY()
	EMRequestType RequestType = EMRequestType::None;

	// 상대에게 받을 골드 혹은 교환할 골드
	UPROPERTY()
	int64 RequiredGold = 0;

	// 상대에게 받을 아이템 혹은 교환할 아이템
	UPROPERTY()
	FMTradeItemInfo RequiredItems;

	// 상대에게 줄 골드 혹은 교환할 골드
	UPROPERTY()
	int64 GrantGold = 0;

	// 상대에게 줄 아이템 혹은 교환할 아이템
	UPROPERTY()
	FMTradeItemInfo GrantItems;

	// 기타 설명용 문자열
	UPROPERTY()
	FString ContextString;
};

UENUM(BlueprintType)
enum class EMResponseType : uint8
{
	None,
	Error,
	Fail,
	Progress,
	Success,
};

USTRUCT(BlueprintType)
struct FMTradeResponse
{
	GENERATED_BODY()

	int32 TradeId = INDEX_NONE;

	EMResponseType ResponseType = EMResponseType::None;

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

protected:
/*
* Member Variables
*/
protected:
	
};
