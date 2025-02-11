// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "MDataLoadSubsystem.generated.h"

class UDataTable;

/**
 * 
 */
UCLASS()
class PROJECTM_API UMDataLoadSubsystem : public UEngineSubsystem
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
	const UDataTable* GetDataTable(const UScriptStruct* TableRowData) const;
private:
	void LoadDataTables();
	/*
	* Member Variables
	*/
public:
	UPROPERTY(BlueprintReadOnly)
	TMap<const UScriptStruct*, UDataTable*> TableMap;
};
