// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "Table/MTableAsset.h"
#include "Templates/SubclassOf.h"
#include "Inventory/PMInventoryItemDefinition.h"
#include "MDataTableManager.generated.h"

class UDataTable;

/**
 * 
 */
UCLASS()
class PROJECTM_API UMDataTableManager : public UEngineSubsystem
{
	GENERATED_BODY()
/*
* Overrided Function
*/
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

/*
* Member Functions
*/
public:
	static int32 ChangeElementIdToTableId(const FString& ElementId);
	static int32 ChangeElementIdToIndex(const FString& ElementId);


	UFUNCTION(BlueprintCallable)
	const UDataTable* GetDataTable(const FString& ElementId) const;
	//Deprecated
	const UDataTable* GetDataTable(int32 TableId) const;
	//Deprecated
	const UDataTable* GetDataTable(EMItemIdType TableType) const;

	UFUNCTION(BlueprintCallable)
	UPMInventoryItemDefinition*						GetItemDefinition(int32 TableId, int32 ItemId) const;
	const TSubclassOf<UPMInventoryItemDefinition>	GetItemDefinition(EMItemIdType TableType, int32 ItemId) const;

	int32 GetTableNum() const { return Deprecated_TableMap.Num(); }
private:
	void LoadDataTables();
	void OnLoadedDataTables();
	void ParseTableMap(UMTableAsset* TableAsset);

/*
* Member Variables
*/
public:
	UPROPERTY(BlueprintReadOnly)
	TMap<EMItemIdType, UDataTable*> Deprecated_TableMap;

	UPROPERTY(BlueprintReadOnly)
	TMap<int32/*table id*/, UDataTable*> TableMap;

	TArray<FPrimaryAssetId>	PrimaryAssetIdList;
};
