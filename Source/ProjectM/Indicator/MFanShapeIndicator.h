// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MRangeIndicatorActor.h"
#include "MFanShapeIndicator.generated.h"

class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS()
class PROJECTM_API AMFanShapeIndicator : public AMRangeIndicatorActor
{
    GENERATED_BODY()

/*
* Overrided Functions
*/
public:
    AMFanShapeIndicator();

    virtual void    Tick(float DeltaSeconds) override;

protected:
    virtual void    BeginPlay() override;

/*
* Member Functions
*/
public:
    float   GetAngle() const { return Angle; }

    void    SetIndicatorValue(float InAngle) { Angle = InAngle; }
    void    SetAngle(float InAngle) { Angle = InAngle; }

/*
* Member Variables
*/
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Indicator | Value")
    float   Angle = 0;
};