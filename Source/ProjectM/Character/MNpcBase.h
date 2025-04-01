// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MNpcBase.generated.h"

class UWidgetComponent;

/**
 * 
 */
UCLASS()
class PROJECTM_API AMNpcBase : public ACharacter
{
	GENERATED_BODY()
/*
* Overrided Function
*/
public:
	AMNpcBase();

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* NpcName;
};
