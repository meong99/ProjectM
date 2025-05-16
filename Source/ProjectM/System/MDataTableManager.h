// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "Table/MTableAsset.h"
#include "Templates/SubclassOf.h"
#include "Inventory/PMInventoryItemDefinition.h"
#include "Definitions/MDefinitionBase.h"
#include "MDataTableManager.generated.h"

class UDataTable;
class UMMonsterDefinition;

UENUM(BlueprintType)
enum class EMTableKey : uint8
{
	None,
	ConsumableItem,
	EquipmentItem,
	Shop,
	Monster,
	NPC,
	Level,
	Quest,
	Dialogue,
	OthersItem,
};

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
	const UDataTable* GetDataTable(EMTableKey Key) const;

	UFUNCTION(BlueprintCallable)
	UPMInventoryItemDefinition*	GetItemDefinition(int32 RowId) const;
	UFUNCTION(BlueprintCallable)
	UMMonsterDefinition*		GetMonsterDefinition(int32 RowId) const;
	template<class T>
	const TSubclassOf<T>		GetDefinitionClass(int32 RowId) const;

	int32 GetTableNum() const { return TableMap.Num(); }
private:
	void LoadDataTables();
	void OnLoadedDataTables();
	void ParseTableMap(UMTableAsset* TableAsset);

/*
* Member Variables
*/
public:
	UPROPERTY(BlueprintReadOnly)
	TMap<int32/*table id*/, UDataTable*> TableMap;

	TArray<FPrimaryAssetId>	PrimaryAssetIdList;
};

template<class T>
const TSubclassOf<T> UMDataTableManager::GetDefinitionClass(int32 RowId) const
{
	const UDataTable* DataTable = GetDataTable(RowId);
	if (DataTable)
	{
		int32 ElementIndex = ChangeRowIdToElementId(RowId) - 1;
		const TArray<FName>& Names = DataTable->GetRowNames();
		if (Names.IsValidIndex(ElementIndex))
		{
			FMTable_TableBase* RowData = DataTable->FindRow<FMTable_TableBase>(Names[ElementIndex], Names[ElementIndex].ToString());
			if (RowData)
			{
				return RowData->Definition.Get();
			}
		}
	}

	return nullptr;
}
