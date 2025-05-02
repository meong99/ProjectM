// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "MWidgetBase.generated.h"

UENUM(BlueprintType)
enum class EMWidgetInputMode : uint8
{
	GameAndUI,
	GameOnly,
	UIOnly,
	GameAndUIWithShowMouse,
};

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
	UFUNCTION(BlueprintCallable)
	void AddWidgetToLayer(const int32 LayerId = 0/*GameLayer*/);
	UFUNCTION(BlueprintCallable)
	void RemoveWidgetFromLayer(const int32 LayerId = 0/*GameLayer*/);

	// 생성되고 레이어에 등록되기 직전 한 번만 호출됨. WidgetInstigator등 커스텀 변수 사용한다면 NativeOnInitialized함수 대신 사용
	virtual void PreAddToLayer() {}

	const FGameplayTag& GetWidgetTag() const { return WidgetTag; }
	bool				IsActivate() const { return bIsActivate; }
	EMWidgetInputMode	GetInputMode() const { return InputMode; }
	UObject*			GetWidgetInstigator() const { return WidgetInstigator; }

	void SetWidgetTag(const FGameplayTag& InWidgetTag) { WidgetTag = InWidgetTag; }
	void SetActivate(const bool bNewActivate) { bIsActivate = bNewActivate; }
	void SetWidgetInstigator(UObject* InInstigator) { WidgetInstigator = InInstigator; }
	UFUNCTION(BlueprintCallable)
	bool IsInLayer() const { return bIsActivate; }
	bool IsInitialized() const { return bIsInitialized; }
/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly)
	EMWidgetInputMode InputMode = EMWidgetInputMode::GameOnly;

	UPROPERTY(VisibleInstanceOnly)
	FGameplayTag WidgetTag;

	// Layer에 등록된 위젯의 활성화 여부
	UPROPERTY(BlueprintReadOnly)
	bool bIsActivate = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsInitialized = false;

	UPROPERTY(BlueprintReadWrite)
	UObject* WidgetInstigator;
};
