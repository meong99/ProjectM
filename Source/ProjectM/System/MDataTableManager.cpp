// Fill out your copyright notice in the Description page of Project Settings.

#include "System/MDataTableManager.h"
#include "DataAssets/PMAssetManager.h"
#include "Engine/Engine.h"
#include "Table/Item/MTable_ConsumableItem.h"
#include "Misc/MessageDialog.h"

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
	return TableMap.FindRef(TableType);
}


const TSubclassOf<UPMInventoryItemDefinition> UMDataTableManager::GetItemDefinition(EMItemIdType TableType, int32 ItemId) const
{
	const UDataTable* DataTable = GetDataTable(TableType);
	if (DataTable)
	{
		const TArray<FName>& Names = DataTable->GetRowNames();
		if (Names.IsValidIndex(ItemId))
		{
			FMTable_ItemBase* Item = DataTable->FindRow<FMTable_ItemBase>(Names[ItemId], Names[ItemId].ToString());
			if (Item)
			{
				return Item->ItemDefinition;
			}
		}
	}

	return nullptr;
}

void UMDataTableManager::LoadDataTables()
{
	UPMAssetManager& AssetManager = UPMAssetManager::Get();
	check(UPMAssetManager::IsInitialized());

	FPrimaryAssetType		AssetType{ UMTableAsset::StaticClass()->GetFName() };
	TArray<FPrimaryAssetId>	PrimaryAssetIdList;
	AssetManager.GetPrimaryAssetIdList(AssetType, PrimaryAssetIdList);

	TArray<FName> LoadBundles;
	AssetManager.LoadPrimaryAssets(PrimaryAssetIdList, LoadBundles, FSimpleMulticastDelegate::FDelegate::CreateLambda(
		[this, PrimaryAssetIdList, &AssetManager]()->void
		{
			for (const FPrimaryAssetId& AssetId : PrimaryAssetIdList)
			{
				UMTableAsset* TableAsset = AssetManager.GetPrimaryAssetObject<UMTableAsset>(AssetId);

				if (IsValid(TableAsset))
				{
					for (const FMTableDefinition& TableDefinition : TableAsset->TableDefinitions)
					{
						if (TableDefinition.TableType != EMItemIdType::None)
						{
							TableMap.Add(TableDefinition.TableType, TableDefinition.Table);
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
		})
	);
}
