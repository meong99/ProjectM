// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "PMActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class EPMWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu,
};

UCLASS()
class PROJECTM_API UPMActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	UPMActivatableWidget(const FObjectInitializer& ObjectInitializer);

	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	EPMWidgetInputMode InputConfig = EPMWidgetInputMode::Default;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
};
