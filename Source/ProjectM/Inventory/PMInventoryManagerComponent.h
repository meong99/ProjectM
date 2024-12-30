#pragma once

#include "CoreMinimal.h"
#include "Components/ControllerComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "PMInventoryManagerComponent.generated.h"

class UPMInventoryItemInstance;
class UPMInventoryItemDefinition;

USTRUCT(BlueprintType)
struct FMInventoryChangeMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UPMInventoryItemInstance> Instance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 Delta = 0;
};

USTRUCT(BlueprintType)
struct FPMInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UPMInventoryItemInstance> Instance = nullptr;
};

USTRUCT(BlueprintType)
struct FPMInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FPMInventoryList();
	FPMInventoryList(UActorComponent* InOwnerComponent);

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FPMInventoryEntry, FPMInventoryList>(Entries, DeltaParms, *this);
	}

	void BroadcastChangeMessage(FPMInventoryEntry& Entry, int32 OldCount, int32 NewCount);

	UPMInventoryItemInstance* AddEntry(TSubclassOf<UPMInventoryItemDefinition> ItemDef);
	void AddEntry(UPMInventoryItemInstance* Instance);
	void RemoveEntry(UPMInventoryItemInstance* Instance);

	// 실제 아이템의 Instance를 보유하고있는 배열
	UPROPERTY()
	TArray<FPMInventoryEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FPMInventoryList> : public TStructOpsTypeTraitsBase2<FPMInventoryList>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};

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

/*
* Member Functions
*/
public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	UPMInventoryItemInstance* AddItemDefinition(TSubclassOf<UPMInventoryItemDefinition> ItemDef);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddItemInstance(UPMInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void RemoveItemInstance(UPMInventoryItemInstance* ItemInstance);
/*
* Member Variables
*/
private:
	UPROPERTY(Replicated)
	FPMInventoryList InventoryList;
};