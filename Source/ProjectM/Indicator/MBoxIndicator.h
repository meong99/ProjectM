// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MRangeIndicatorActor.h"
#include "MBoxIndicator.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTM_API AMBoxIndicator : public AMRangeIndicatorActor
{
    GENERATED_BODY()

public:
    AMBoxIndicator();

    virtual void    Tick(float DeltaSeconds) override;

protected:
    virtual void    BeginPlay() override;

public:

    float   GetLineThickness() const { return LineThickness; }

    void    SetIndicatorValue(float InLineThickness) { LineThickness = InLineThickness; }
    void    SetLineThickness(float InLineThickness) { LineThickness = InLineThickness; }

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Indicator | Value")
    float   LineThickness = 0.05;
};