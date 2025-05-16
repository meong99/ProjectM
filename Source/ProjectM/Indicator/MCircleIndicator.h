// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MRangeIndicatorActor.h"
#include "MCircleIndicator.generated.h"

/**
 *
 */
UCLASS()
class PROJECTM_API AMCircleIndicator : public AMRangeIndicatorActor
{
	GENERATED_BODY()

public:
	AMCircleIndicator();

    virtual void    Tick(float DeltaSeconds) override;

protected:
    virtual void    BeginPlay() override;

public:
    float   GetRadius() const { return Radius; }

    void    SetIndicatorValue(float InRadius) { Radius = InRadius; }
    void    SetRadius(float InRadius) { Radius = InRadius; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Indicator | Value")
    float   Radius = 0;
};