#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "AbilitySystem/PMAbilitySet.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "PMEquipmentManagerComponent.generated.h"

class UPMEquipmentDefinition;
class UPMEquipmentInstance;

USTRUCT(BlueprintType)
struct FPMAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<UPMEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UPMEquipmentInstance> Instance = nullptr;

	// 장비 장착시 부여된 어빌리티들의 핸들
	UPROPERTY(NotReplicated)
	FPMAbilitySet_GrantedHandles GrantedHandles;
};

USTRUCT(BlueprintType)
struct FPMEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FPMEquipmentList();
	FPMEquipmentList(UActorComponent* InOwnerComponent);

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	UPMEquipmentInstance* AddEntry(TSubclassOf<UPMEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UPMEquipmentInstance* Instance);

	UPMAbilitySystemComponent* GetAbilitySystemComponent();

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FPMAppliedEquipmentEntry, FPMEquipmentList>(Entries, DeltaParms, *this);
	}

	UPROPERTY()
	TArray<FPMAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FPMEquipmentList> : public TStructOpsTypeTraitsBase2<FPMEquipmentList>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};

/**
 *
 */
UCLASS()
class PROJECTM_API UPMEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;

/*
* Member Functions
*/
public:
	UPMEquipmentInstance* EquipItem(TSubclassOf<UPMEquipmentDefinition> EquipmentDefinition);
	void UnequipItem(UPMEquipmentInstance* ItemInstance);

	UFUNCTION(BlueprintCallable)
	TArray<UPMEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UPMEquipmentInstance> InstanceType) const;

	const FPMEquipmentList& GetEquipmentList() const { return EquipmentList; }

	UPMEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UPMEquipmentInstance> InstanceType);

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

/*
* Member Variables
*/
private:
	// Pawn에게 장착되어있는 모든 장비에 대한 정보
	UPROPERTY(Replicated)
	FPMEquipmentList EquipmentList;
};