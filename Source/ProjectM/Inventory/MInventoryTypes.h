#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "MInventoryTypes.generated.h"

class UPMInventoryItemInstance;
class UPMInventoryItemDefinition;
class UActorComponent;

USTRUCT(BlueprintType)
struct FMItemHandle 
{
	GENERATED_BODY()

	bool IsValid() const
	{
		return ItemUid != INDEX_NONE;
	}

	int32 ItemUid = INDEX_NONE;
};

USTRUCT(BlueprintType)
struct FPMInventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	bool IsValid() const
	{
		return ItemUid != INDEX_NONE;
	}

	UPROPERTY()
	int32 ItemUid = INDEX_NONE;

	UPROPERTY()
	TObjectPtr<UPMInventoryItemInstance> Instance = nullptr;
};

// 아이템들을 관리하는 객체
USTRUCT(BlueprintType)
struct FPMInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FPMInventoryList();
	FPMInventoryList(UActorComponent* InOwnerComponent);

/*
* Network
*/
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FPMInventoryEntry, FPMInventoryList>(Entries, DeltaParms, *this);
	}

/*
* Member Function
*/
public:
	int32 ChangeItemQuantity(const FMItemHandle& ItemHandle, int32 ChangeNum);

	FMItemHandle AddEntry(TSubclassOf<UPMInventoryItemDefinition> ItemDef);
	void RemoveEntry(const FMItemHandle& ItemHandle);
	FPMInventoryEntry* FindEntry(const FMItemHandle& ItemHandle);
/*
* Member Variables
*/
public:
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
