// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MRangeIndicatorActor.generated.h"

class UDecalComponent;
class UMaterialInstanceDynamic;

UCLASS(Abstract, BlueprintType, Blueprintable)
class PROJECTM_API AMRangeIndicatorActor : public AActor
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	AMRangeIndicatorActor();
	virtual void PreInitializeComponents() override;

/*
* Member Functions
*/
public:
    void    SetIndicatorColor(FColor InColor);

/*
* Member Variables
*/
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UDecalComponent> IndicatorDecal;
	
    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UMaterialInstanceDynamic> IndicatorInstance;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FColor                      IndicatorColor = FColor::Red;
};