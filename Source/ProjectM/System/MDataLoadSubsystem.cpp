// Fill out your copyright notice in the Description page of Project Settings.

#include "System/MDataLoadSubsystem.h"
#include "DataAssets/PMAssetManager.h"
#include "Table/MTableAsset.h"
#include "Engine/Engine.h"
#include "Table/Item/MTable_ConsumableItem.h"

void UMDataLoadSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UAssetManager::CallOrRegister_OnCompletedInitialScan(FSimpleMulticastDelegate::FDelegate::CreateLambda(
		[this]()->void
		{
			LoadDataTables();
		})
	);
}

const UDataTable* UMDataLoadSubsystem::GetDataTable(const UScriptStruct* TableRowData) const
{
	return TableMap.FindRef(TableRowData);
}

void UMDataLoadSubsystem::LoadDataTables()
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
					for (UDataTable* Table :TableAsset->Tables)
					{
						const UScriptStruct* TableRowStruct = Table->GetRowStruct();
						if (TableRowStruct)
						{
							TableMap.Add(TableRowStruct, Table);
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
