// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MWidgetBase.h"
#include "MWidgetLayout.generated.h"

class UOverlay;
class UWidgetSwitcher;
class UMWidgetLayer;

UENUM(BlueprintType)
enum class EMWidgetLayout : uint8
{
	// 게임 플레이 중 나오는 Layout. (HUD, Inventory ...)
	GameLayer UMETA(DisplayName = "GameLayer"),

	// 다른 위젯 없이 단독으로 보여지는 위젯 레이아웃. 단 하나씩만 설정 가능하다.
	IndependentLayer UMETA(DisplayName = "IndependentLayer"),

	None UMETA(DisplayName = "None"),
};

UCLASS()
class PROJECTM_API UMWidgetLayout : public UMWidgetBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMWidgetLayout(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

/*
* Member Functions
*/
public:
	void ChangeWidgetLayer(EMWidgetLayout WidgetLayout) const;
	void ClearAllWidget();

	void AddWidgetToCurrentLayer(UMWidgetBase* Widget) const;
	void RemoveWidgetToCurrentLayer(UMWidgetBase* Widget) const;
	void AddWidgetToLayer(UMWidgetBase* Widget, EMWidgetLayout WidgetLayout) const;
	void RemoveWidgetFromLayer(UMWidgetBase* Widget, EMWidgetLayout WidgetLayout) const;

private:
	UMWidgetLayer* GetLayer(EMWidgetLayout WidgetLayout) const;
/*
* Member Variables
*/
protected:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* LayoutSwitcher;

	UPROPERTY(meta = (BindWidget))
	UMWidgetLayer* GameLayer;

	UPROPERTY(meta = (BindWidget))
	UMWidgetLayer* IndependentLayer;
};
