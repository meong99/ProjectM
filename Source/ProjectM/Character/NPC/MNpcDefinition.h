// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MNpcDefinition.generated.h"

class UMNameWidgetComponent;

/**
 *
 */
UCLASS(Abstract, Blueprintable, DefaultToInstanced, EditInlineNew)
class PROJECTM_API UMNpcDefinition : public UObject
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMNpcDefinition();

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
public:
	UPROPERTY(EditDefaultsOnly)
	FName NpcName;
};
