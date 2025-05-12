#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "Inventory/PMInventoryItemList.h"

#include "PMInventoryManagerComponent.generated.h"

class ULocalPlayerSaveGame;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnNewItemAdded, const FPMInventoryEntry& ItemEntry, const FMItemResponse& ItemRespons);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemIncreased, const FMItemResponse& ItemRespons);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemDecreased, const FMItemResponse& ItemRespons);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeItemQuentity, const FMItemHandle& ItemHandle, const FMItemResponse& ItemRespons);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRemoveItem, const FMItemHandle& ItemHandle, const EMItemType ItemType);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInitInventory, const FPMInventoryItemList& InventoryList);

/**
 * InventoryManager는 Controller에서 아이템에 관한 메타데이터 및 Instance를 들고있고, 실제 Actor(무기)자체를 스폰하는거는 Character에 부착된 EquipmentManagerComponent에서 한다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTM_API UPMInventoryManagerComponent : public UControllerComponent
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~End of UObject interface

	virtual void InitializeComponent() override;

/*
* Member Functions
*/
public:
	void CallOrRegister_OnInitInventory(FOnInitInventory::FDelegate&& Delegate);

	// 아이템을 추가하고 Instancing해서 저장한다.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	FMItemHandle	RequestItemToInventory(const FMItemRequest& ItemRequest);

	FDelegateHandle AddDelegateOnChangeItemQuentity(const int32 ItemUid, FOnChangeItemQuentity::FDelegate&& Delegate);
	void			RemoveDelegateOnChangeItemQuentity(const int32 ItemUid, const FDelegateHandle& DelegateHandle);

	UFUNCTION(Server, Reliable)
	void Server_UseItem(const FMItemHandle& ItemHandle);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UPMInventoryItemInstance*	FindItemInstance(const FMItemHandle& ItemHandle);
	FPMInventoryEntry*			FindEntry(const FMItemHandle& ItemHandle);
	FPMInventoryEntry*			FindEntry(TSubclassOf<UPMInventoryItemDefinition> ItemDef);

	int32 GetItemQuantity(const int32 ItemRowId);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	int32						GetMaxInventoryCount() const { return MaxInventoryCount; }
	const FPMInventoryItemList& GetEquipmentItemList() const { return InventoryList; }
	const FPMInventoryItemList& GetConsumableItemList() const { return ConsumableItemList; }

protected:
	UFUNCTION()
	void	InitInventory();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	int32	ChangeItemQuantity(const FMItemHandle& ItemHandle, const FMItemRequest& ItemRequest);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void	RemoveItem(const FMItemHandle& ItemHandle);
	
	FMItemHandle	AddItemDefinition_Impl(TSubclassOf<UPMInventoryItemDefinition> ItemDef, FPMInventoryItemList& ItemList, const FMItemRequest& ItemRequest);
	FMItemHandle	ReturnItem(UPMInventoryItemInstance* Instance);

	UPMInventoryItemDefinition* GetItemDefCDO(const int32 ItemRowId);
	UPMInventoryItemDefinition* GetItemDefCDO(const TSubclassOf<UPMInventoryItemDefinition>& ItemDef);
	UPMInventoryItemDefinition* GetItemDefCDO(const FMItemRequest& ItemRequest);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnNewItemAdded(const FPMInventoryEntry& ItemEntry, const FMItemResponse& ItemRespons);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnItemIncreased(const FMItemResponse& ItemRespons);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnItemDecreased(const FMItemResponse& ItemRespons);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnChangeInventory(const FMItemHandle& ItemHandle, const FMItemResponse& ItemRespons);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnRemoveItem(const FMItemHandle& ItemHandle, const EMItemType ItemType);

	FPMInventoryItemList* GetItemList(const EMItemType ItemType);
/*
* Member Variables
*/
public:
	FOnRemoveItem		Delegate_OnRemoveItem;
	FOnNewItemAdded		Delegate_OnNewItemAdded;
	FOnItemIncreased	Delegate_OnItemIncreased;
	FOnItemDecreased	Delegate_OnItemDecreased;
protected:
	// 인벤토리같이 다수의 객체에서 아이템 변경을 추적하지만, 특정 ItemUID에 대한 알림만 받고싶을 경우에 사용
	TMap<int32, FOnChangeItemQuentity>	DelegateMap_OnChangeItemQuentity;
	
private:
	UPROPERTY(Replicated)
	FPMInventoryItemList InventoryList;

	UPROPERTY(Replicated)
	FPMInventoryItemList ConsumableItemList;

#pragma TODO("하드코딩 말고 데이터 읽기 만들면 변경해야함")
	UPROPERTY(BlueprintReadOnly, meta=(AllowprivateAccess=true))
	int32 MaxInventoryCount = 30;

	bool bIsInitInventory = false;

	FOnInitInventory Delegate_OnInitInventory;

// DEBUG
private:
#if WITH_EDITOR
	UFUNCTION(Exec)
	void Debug_AddItem(int32 RowId, int32 ItemQuentity = 1);
	UFUNCTION(Server, Reliable)
	void DebugServer_AddItem(int32 Rowid, int32 ItemQuentity);
#endif
};