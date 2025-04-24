// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "MWalletComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeGold, int64, AdjustNum, int64, NewGold);

/*
* 입력에 따라 위젯이 켜지고, 꺼지는 동작을 수행할 수 있도록 하는 컴포넌트
*/
UCLASS()
class PROJECTM_API UMWalletComponent : public UControllerComponent
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMWalletComponent(const FObjectInitializer& ObjectInitializer);

/*
* Member Functions
*/
public:
	UFUNCTION(BlueprintCallable)
	void SubtractGold(int64 AdjustGold);

	UFUNCTION(BlueprintCallable)
	void AddGold(int64 AdjustGold);

	UFUNCTION(BlueprintCallable)
	int64 GetGold() const { return Gold; }

#if WITH_EDITOR
	UFUNCTION(Exec)
	void Debug_AddGold(int64 AdjustGold);
	UFUNCTION(Exec)
	void Debug_SubtractGold(int64 AdjustGold);
#endif
/*
* Member Variables
*/
public:
	UPROPERTY(BlueprintAssignable)
	FOnChangeGold Delegate_OnChangeGold;

protected:
	UPROPERTY(meta=(ClampMin=0))
	int64 Gold = 0;
};
