// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MDragableWidget.generated.h"

class UImage;
class UMContextableItemWidget;

UCLASS()
class PROJECTM_API UMDragableWidget : public UMWidgetBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMDragableWidget(const FObjectInitializer& ObjectInitializer);

	virtual void	NativeOnInitialized() override;
	virtual FReply	NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void	NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool	NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void	NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void	NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void	NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void	NativeDestruct() override;

	/*
* Member Functions
*/
private:
	void EnableContextWidget() const;
	void DisableContextWidget() const;
/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UImage> ItemImage;

	UPROPERTY(BlueprintReadOnly)
	int32 ItemRowId = INDEX_NONE;
};
