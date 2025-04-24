// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "Table/MTableAsset.h"
#include "Templates/SubclassOf.h"
#include "Inventory/PMInventoryItemDefinition.h"
#include "MDataTableManager.generated.h"

class UDataTable;
class UMMonsterDefinition;

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
	static int32 ChangeElementIdToTableId(int32 RowId);
	static int32 ChangeRowIdToElementId(int32 RowId);

	const UDataTable* GetDataTable(int32 RowId) const;
	//Deprecated
	const UDataTable* GetDataTable(EMItemIdType TableType) const;

	UFUNCTION(BlueprintCallable)
	UPMInventoryItemDefinition*						GetItemDefinition(int32 RowId, FString ContextString) const;
	const TSubclassOf<UPMInventoryItemDefinition>	GetItemDefinition(int32 RowId) const;
	UFUNCTION(BlueprintCallable)
	UMMonsterDefinition* GetMonsterDefinition(int32 RowId) const;

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
