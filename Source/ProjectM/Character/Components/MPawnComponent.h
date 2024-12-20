// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "MPawnComponent.generated.h"


/*
* 입력에 따라 위젯이 켜지고, 꺼지는 동작을 수행할 수 있도록 하는 컴포넌트
*/
UCLASS()
class PROJECTM_API UMPawnComponent : public UPawnComponent
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMPawnComponent(const FObjectInitializer& ObjectInitializer);

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
protected:
};
