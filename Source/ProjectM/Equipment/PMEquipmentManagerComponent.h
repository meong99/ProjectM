#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "AbilitySystem/PMAbilitySet.h"

#include "PMEquipmentManagerComponent.generated.h"

class UPMEquipmentDefinition;
class UPMEquipmentInstance;

USTRUCT(BlueprintType)
struct FPMAppliedEquipmentEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<UPMEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UPMEquipmentInstance> Instance = nullptr;

	// 장비 장착시 부여된 어빌리티들의 핸들
	UPROPERTY()
	FPMAbilitySet_GrantedHandles GrantedHandles;
};

USTRUCT(BlueprintType)
struct FPMEquipmentList
{
	GENERATED_BODY()

	FPMEquipmentList();
	FPMEquipmentList(UActorComponent* InOwnerComponent);

	UPMEquipmentInstance* AddEntry(TSubclassOf<UPMEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UPMEquipmentInstance* Instance);

	UPMAbilitySystemComponent* GetAbilitySystemComponent();

	UPROPERTY()
	TArray<FPMAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
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
	UPROPERTY()
	FPMEquipmentList EquipmentList;
};