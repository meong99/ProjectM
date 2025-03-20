// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MWidgetBase.h"
#include "MWidgetLayout.generated.h"

class UOverlay;
class UWidgetSwitcher;

UENUM(BlueprintType)
enum class EMWidgetLayout : uint8
{
	// 게임 플레이 중 나오는 Layout. (HUD, Inventory ...)
	GameLayout UMETA(DisplayName = "GameLayout"),

	// 옵션 및 메뉴 등 게임 플레이에 영향을 주지 않는 layout.
	MenuLayout UMETA(DisplayName = "MenuLayout"),

	// 상점 및 기타 위젯끼리 상호작용할 때 사용할 layout
	ShopLayout UMETA(DisplayName = "ShopLayout"),

	// 미정. 기타 등등
	OtherLayout UMETA(DisplayName = "OtherLayout"),


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

/*
* Member Functions
*/
public:
	void ChangeWidgetLayout(EMWidgetLayout WidgetLayout) const;

	void AddWidgetToCurrentLayout(UMWidgetBase* Widget) const;
	void RemoveWidgetToCurrentLayout(UMWidgetBase* Widget) const;
	void AddWidgetToLayout(UMWidgetBase* Widget, EMWidgetLayout WidgetLayout) const;
	void RemoveWidgetFromLayout(UMWidgetBase* Widget, EMWidgetLayout WidgetLayout) const;

protected:
	void SetInputMode(uint8 NewInputMode) const;

private:
	UOverlay* GetLayout(EMWidgetLayout WidgetLayout) const;
/*
* Member Variables
*/
protected:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* LayoutSwitcher;

	UPROPERTY(meta = (BindWidget))
	UOverlay* GameLayout;

	UPROPERTY(meta = (BindWidget))
	UOverlay* MenuLayout;

	UPROPERTY(meta = (BindWidget))
	UOverlay* ShopLayout;

	UPROPERTY(meta = (BindWidget))
	UOverlay* OtherLayout;
};
