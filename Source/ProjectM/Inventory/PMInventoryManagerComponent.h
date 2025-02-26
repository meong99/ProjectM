#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "Inventory/PMInventoryItemList.h"

#include "PMInventoryManagerComponent.generated.h"

class ULocalPlayerSaveGame;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeInventory, const FMItemHandle& ItemHandle);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNewItemAdded, const FPMInventoryEntry& ItemEntry);
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
	FMItemHandle	AddItemDefinition(TSubclassOf<UPMInventoryItemDefinition> ItemDef);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	int32			ChangeItemQuantity(const FMItemHandle& ItemHandle, int32 ChangeNum);
	FDelegateHandle AddDelegateOnChangeInventory(const int32 ItemUid, FOnChangeInventory::FDelegate&& Delegate);
	void			RemoveDelegateOnChangeInventory(const int32 ItemUid, const FDelegateHandle& DelegateHandle);

	UFUNCTION(Server, Reliable)
	void Server_UseItem(const FMItemHandle& ItemHandle);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UPMInventoryItemInstance*	FindItemInstance(const FMItemHandle& ItemHandle);
	FPMInventoryEntry*			FindEntry(const FMItemHandle& ItemHandle);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	int32						GetMaxInventoryCount() const { return MaxInventoryCount; }
	const FPMInventoryItemList& GetEquipmentItemList() const { return InventoryList; }
	const FPMInventoryItemList& GetConsumableItemList() const { return ConsumableItemList; }

protected:
	FMItemHandle AddItemDefinition_Impl(TSubclassOf<UPMInventoryItemDefinition> ItemDef, FPMInventoryItemList& ItemList);

	UFUNCTION()
	void InitInventory();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void RemoveItem(const FMItemHandle& ItemHandle);

	FPMInventoryItemList* GetItemList(const EMItemType ItemType);
/*
* Member Variables
*/
public:
	FOnNewItemAdded Delegate_OnNewItemAdded;
	TMap<int32, FOnChangeInventory> Delegate_OnChangeInventory;
	
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
	UFUNCTION(Exec)
	void Debug_AddItem(int32 TableId, int32 ItemId);
	UFUNCTION(Server, Reliable)
	void DebugServer_AddItem(TSubclassOf<UPMInventoryItemDefinition> ItemDef);
};