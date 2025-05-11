// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MQuestComponentBase.h"
#include "MNpcQuestComponent.generated.h"

/**
 *
 */
UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class PROJECTM_API UMNpcQuestComponent : public UMQuestComponentBase
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMNpcQuestComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

/*
* Member Functions
*/
public:
	
/*
* Member Variables
*/
protected:
	
};
