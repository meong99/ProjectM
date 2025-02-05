// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "Util/MPriorityQueue.h"
#include "Inventory/PMInventoryItemList.h"
#include "MInventoryWidget.generated.h"

class UTileView;
class UPMInventoryManagerComponent;
class UMItemTileWidget;

struct FPMInventoryEntry;
class UButton;
class UWidgetSwitcher;

UENUM(BlueprintType)
enum class EMInventoryType : uint8
{
	Equipment,
	Consumable,
};

UCLASS()
class PROJECTM_API UMInventoryWidget : public UMWidgetBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMInventoryWidget(const FObjectInitializer& ObjectInitializer);

	virtual void	NativeOnInitialized() override;
	virtual FReply	NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply	NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

/*
* Member Functions
*/
public:
	void	Callback_OnInitInventory(const FPMInventoryItemList& InventoryList);

	//인벤토리에 존재하지 않던 새로운 아이템이 추가됨
	void	Callback_AddNewItem(const FPMInventoryEntry* NewItemEntry);

	void	RegisterEmptySlot(MPriorityQueueNode<UMItemTileWidget>&& NewNode);

protected:
	void	BindDelegates();
	void	InitInventorySlots(const FPMInventoryItemList& InventoryList);
	void	InitInventorySlots_Impl(const FPMInventoryItemList& InventoryList, UTileView* ItemSlots);

	UTileView* GetItemSlotView(const EMItemType ItemType);

	UFUNCTION()
	void	OnClick_EquipmentButton();
	UFUNCTION()
	void	OnClick_ConsumableButton();
	UFUNCTION()
	void	OnClick_ExitButton();

protected:
	MPriorityQueueNode<UMItemTileWidget> PopEmptySlot();
/*
* Member Variables
*/
protected:
	MPriorityQueue<UMItemTileWidget> EmptySlots;

	UPROPERTY()
	UPMInventoryManagerComponent* InventoryComponent;

	UPROPERTY(meta = (BindWidget))
	UTileView* TileView_EquipmentItems;

	UPROPERTY(meta = (BindWidget))
	UTileView* TileView_ConsumableItems;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UButton* EquipmentButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ConsumableButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
};
