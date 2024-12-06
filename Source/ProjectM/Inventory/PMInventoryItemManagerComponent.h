#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PMInventoryItemManagerComponent.generated.h"

class UPMInventoryItemInstance;
class UPMInventoryItemDefinition;

USTRUCT(BlueprintType)
struct FPMInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UPMInventoryItemInstance> Instance = nullptr;
};

USTRUCT(BlueprintType)
struct FPMInventoryList
{
	GENERATED_BODY()

	FPMInventoryList();
	FPMInventoryList(UActorComponent* InOwnerComponent);

	UPMInventoryItemInstance* AddEntry(TSubclassOf<UPMInventoryItemDefinition> ItemDef);

	UPROPERTY()
	TArray<FPMInventoryEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

/**
 *
 */
UCLASS()
class PROJECTM_API UPMInventoryItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMInventoryItemManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

/*
* Member Functions
*/
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UPMInventoryItemInstance* AddItemDefinition(TSubclassOf<UPMInventoryItemDefinition> ItemDef);

/*
* Member Variables
*/
private:
	UPROPERTY()
	FPMInventoryList InventoryList;
};