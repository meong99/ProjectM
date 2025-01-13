// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "MWidgetBase.generated.h"

UCLASS()
class PROJECTM_API UMWidgetBase : public UUserWidget
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMWidgetBase(const FObjectInitializer& ObjectInitializer);

	virtual void	NativeOnInitialized() override;
// 	virtual bool	NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

/*
* Member Functions
*/
public:
	const FGameplayTag& GetWidgetTag() const { return WidgetTag; }
	bool				IsActivate() const { return bIsActivate; }

	void SetWidgetTag(const FGameplayTag& InWidgetTag) { WidgetTag = InWidgetTag; }
	void SetActivate(const bool bNewActivate) { bIsActivate = bNewActivate; }
/*
* Member Variables
*/
protected:
	UPROPERTY(VisibleInstanceOnly)
	FGameplayTag WidgetTag;

	// Layer에 등록된 위젯의 활성화 여부
	UPROPERTY(BlueprintReadOnly)
	bool bIsActivate = false;
};
