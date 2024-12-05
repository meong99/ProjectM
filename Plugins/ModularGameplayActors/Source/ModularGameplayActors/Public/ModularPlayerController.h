// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ModularPlayerController.generated.h"

/**
 * 자동으로 Component를 부착하기 위해선 GFCM에 등록하고, 제거해줘야하는데 일일이 하나하나 부착하긴 힘드니 클래스로 빼버림
 */
UCLASS()
class MODULARGAMEPLAYACTORS_API AModularPlayerController : public APlayerController
{
	GENERATED_BODY()

/*
* Overrided Function
*/
protected:
	virtual void PreInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
private:
};
