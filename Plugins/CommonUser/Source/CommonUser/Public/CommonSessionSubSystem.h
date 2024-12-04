// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonSessionSubSystem.generated.h"

UCLASS(BlueprintType)
class COMMONUSER_API UCommonSession_HostSessionRequest : public UObject
{
	GENERATED_BODY()
public:
	FString GetMapName() const;

	// 최종적으로 접속할 데디의 주소.(싱글이라면 맵 이름 및 옵션)
	FString ConstructTravelURL() const;

public:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Session")
	FPrimaryAssetId MapId;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Session")
	TMap<FString, FString> ExtraArgs;
};

/**
 *	Map이동 및 네트워크 관련 작업들을 이 곳에서 추가.
 */
UCLASS()
class COMMONUSER_API UCommonSessionSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UCommonSessionSubSystem() = default;

	UFUNCTION(BlueprintCallable, Category = "Session")
	void HostSession(APlayerController* HostingPlayer, UCommonSession_HostSessionRequest* Request);

private:
	FString PendingTravelURL;
};
