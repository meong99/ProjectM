// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MFloatingWidget.generated.h"

UCLASS()
class PROJECTM_API UMFloatingWidget : public UMWidgetBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMFloatingWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

/*
* Member Functions
*/
public:
protected:
	virtual FReply	NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply	NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply	NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	/*
* Member Variables
*/
protected:
	bool bIsFloating = false;

	FVector2d DragOffset;

	UWidget* TargetWidget = nullptr;
};
