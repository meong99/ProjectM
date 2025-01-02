#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "Inventory/MInventoryTypes.h"

#include "PMInventoryManagerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangeInventory, const FMItemHandle& ItemHandle);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNewItemAdded, const FPMInventoryEntry* ItemEntry);

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
	void CallOrRegister_FinishInventoryInit(FSimpleMulticastDelegate::FDelegate&& Delegate);

	// 아이템을 추가하고 Instancing해서 저장한다.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	FMItemHandle AddItemDefinition(TSubclassOf<UPMInventoryItemDefinition> ItemDef);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UPMInventoryItemInstance* GetItemInstance(const FMItemHandle& ItemHandle);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	int32 ChangeItemQuantity(const FMItemHandle& ItemHandle, int32 ChangeNum);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	int32 GetMaxInventoryCount() const { return MaxInventoryCount; }

	FDelegateHandle AddDelegateOnChangeInventory(const int32 ItemUid, FOnChangeInventory::FDelegate&& Delegate);
	void RemoveDelegateOnChangeInventory(const int32 ItemUid, const FDelegateHandle& DelegateHandle);

protected:
	void InitInventory();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void RemoveItem(const FMItemHandle& ItemHandle);

/*
* Member Variables
*/
public:
	FOnNewItemAdded Delegate_OnNewItemAdded;

private:
	UPROPERTY(Replicated)
	FPMInventoryList InventoryList;

#pragma TODO("하드코딩 말고 데이터 읽기 만들면 변경해야함")
	UPROPERTY(BlueprintReadOnly, meta=(AllowprivateAccess=true))
	int32 MaxInventoryCount = 30;

	TMap<int32, FOnChangeInventory> Delegate_OnChangeInventory;

	bool bIsInitInventory = false;

	FSimpleMulticastDelegate Delegate_FinishInventoryInit;
};