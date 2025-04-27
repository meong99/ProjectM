// Fill out your copyright notice in the Description page of Project Settings.

#include "System/MDataTableManager.h"
#include "DataAssets/PMAssetManager.h"
#include "Engine/Engine.h"
#include "Table/Item/MTable_ConsumableItem.h"
#include "Misc/MessageDialog.h"
#include "Table/Item/MTable_ItemBase.h"
#include "Table/MTable_TableBase.h"
#include "Util/MGameplayStatics.h"
#include "Table/MTable_MonsterTable.h"
#include "Definitions/MMonsterDefinition.h"

void UMDataTableManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UAssetManager::CallOrRegister_OnCompletedInitialScan(FSimpleMulticastDelegate::FDelegate::CreateLambda(
		[this]()->void
		{
			LoadDataTables();
		})
	);
}

const UDataTable* UMDataTableManager::GetDataTable(EMItemIdType TableType) const
{
	return Deprecated_TableMap.FindRef(TableType);
}

const UDataTable* UMDataTableManager::GetDataTable(int32 RowId) const
{
	int32 Key = ChangeElementIdToTableId(RowId);
	return TableMap.FindRef(Key);
}

UPMInventoryItemDefinition* UMDataTableManager::GetItemDefinition(int32 RowId) const
{
	const TSubclassOf<UPMInventoryItemDefinition>& ItemDefinition = GetDefinitionClass<UPMInventoryItemDefinition>(RowId);
	if (ItemDefinition)
	{
		return Cast<UPMInventoryItemDefinition>(DuplicateObject(ItemDefinition->GetDefaultObject<UPMInventoryItemDefinition>(), GetTransientPackage()));
	}
	
	return nullptr;
}

UMMonsterDefinition* UMDataTableManager::GetMonsterDefinition(int32 RowId) const
{
	const TSubclassOf<UMMonsterDefinition>& MonsterDefinition = GetDefinitionClass<UMMonsterDefinition>(RowId);
	if (MonsterDefinition)
	{
		return Cast<UMMonsterDefinition>(DuplicateObject(MonsterDefinition->GetDefaultObject<UMMonsterDefinition>(), GetTransientPackage()));
	}

	return nullptr;
}

int32 UMDataTableManager::ChangeElementIdToTableId(int32 RowId)
{
	int32 Temp = RowId / 100000;
	int32 TableId = INDEX_NONE;
	if (!Temp)
	{
		MCHAE_ERROR("RowId is not valid!!!!!id = % d", RowId);
		return TableId;
	}

	while (Temp)
	{
		TableId += Temp % 10;
		Temp /= 10;
	}

	return TableId;
}

int32 UMDataTableManager::ChangeRowIdToElementId(int32 RowId)
{
	int32 ElementId = INDEX_NONE;

	if (!(RowId / 100000))
	{
		MCHAE_ERROR("RowId is not valid!!!!!id = % d", RowId);
		return ElementId;
	}

	ElementId = RowId % 100000;

	return ElementId;
}

void UMDataTableManager::LoadDataTables()
{
	UPMAssetManager& AssetManager = UPMAssetManager::Get();
	check(UPMAssetManager::IsInitialized());

	FPrimaryAssetType		AssetType{ UMTableAsset::StaticClass()->GetFName() };
	AssetManager.GetPrimaryAssetIdList(AssetType, PrimaryAssetIdList);

	TArray<FSoftObjectPath> Paths;
	AssetManager.GetPrimaryAssetPathList(AssetType, Paths);
	AssetManager.GetStreamableManager().RequestSyncLoad(Paths);
	OnLoadedDataTables();
// 	AssetManager.LoadPrimaryAssets(PrimaryAssetIdList, {}, FStreamableDelegate::CreateUObject(this, &ThisClass::OnLoadedDataTables));
}

void UMDataTableManager::OnLoadedDataTables()
{
	UPMAssetManager& AssetManager = UPMAssetManager::Get();

	for (const FPrimaryAssetId& AssetId : PrimaryAssetIdList)
	{
		UMTableAsset* TableAsset = AssetManager.GetPrimaryAssetObject<UMTableAsset>(AssetId);

		if (IsValid(TableAsset))
		{
			ParseTableMap(TableAsset);


			// ↓ Deprecated
			for (const FMTableDefinition& TableDefinition : TableAsset->TableDefinitions)
			{
				if (TableDefinition.TableType != EMItemIdType::None)
				{
					Deprecated_TableMap.Add(TableDefinition.TableType, TableDefinition.Table);
				}
				else
				{
#if WITH_EDITOR
					FMessageDialog::Open(EAppMsgType::Ok,
						FText::FromString(TEXT("Table Data Error! - TableName : \"") + TableAsset->GetName() + TEXT("\"\n테이블 데이터가 비정상적입니다. 테이블이 비어있는지, 타입은 설정됐는지 확인하세요.")));
#else
					MCHAE_ERROR("Table Data Error! - TableName : \"%s\", Check the TableAsset is currently setted or tabletype is not setted", *TableAsset->GetName());
#endif
				}
			}
		}
		else
		{
			MCHAE_ERROR("Can't Get TableAsset");
		}
	}
}

void UMDataTableManager::ParseTableMap(UMTableAsset* TableAsset)
{
	for (const FMTableDefinition& TableDefinition : TableAsset->TableDefinitions)
	{
		UDataTable* DataTable = TableDefinition.Table;
		if (DataTable == nullptr)
		{
			UMGameplayStatics::ShowErrorOrLog(TEXT("Table Data Error!\n 비어있는 데이터테이블이 있습니다!"));
			continue;
		}
		TArray<FMTable_TableBase*> TableArray;
		DataTable->GetAllRows<FMTable_TableBase>(TEXT("UMDataTableManager::ParseTableMap"), TableArray);

		int32 Key = INDEX_NONE;
		int32 Index = 1;
		for (const FMTable_TableBase* TableRow : TableArray)
		{
			if (!TableRow)
			{
				UMGameplayStatics::ShowErrorOrLog(
					FString::Printf(TEXT("Table Data Error! - TableName : %s\"\"\n 테이블은 모두 FMTable_TableBase의 하위여야합니다"), *DataTable->GetName()));
				continue;
			}
			if (!TableRow->IsValidId())
			{
				UMGameplayStatics::ShowErrorOrLog(
					FString::Printf(TEXT("Table Data Error! - TableName : %s\"\"\n테이블 속성의 ID가 잘못됐습니다!\n속성의 ID는 x..yyyyy의 형태여야합니다! Id = %d"), *DataTable->GetName(), TableRow->RowId));
				continue;
			}
			if (TableRow->GetElementId() != Index)
			{
				UMGameplayStatics::ShowErrorOrLog(
						FString::Printf(TEXT("Table Data Error! - TableName : %s\"\"\n테이블 속성의 순서가 맞지 않습니다! 테이블 속성은 1~n까지 순서대로 있어야합니다!\nNumber%d"),Index));
				continue;
			}

			Index++;
			if (Key == INDEX_NONE)
			{
				Key = TableRow->GetKey();
			}
		}

		if (Key != INDEX_NONE)
		{
			TableMap.Emplace(Key, DataTable);
		}
	}
}
