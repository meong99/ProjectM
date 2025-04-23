#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MTable_TableBase.generated.h"

/**
 *
 */
USTRUCT(BlueprintType, Blueprintable)
struct FMTable_TableBase : public FTableRowBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) override;

/*
* Member Functions
*/
public:
	bool IsValidId() const;
	int32 GetKey() const;
	int32 GetElementId() const;
/*
* Member Variables
*/
public:
	static TMap<int32/*table id*/, FMTable_TableBase*> TableMap;

	// xxxyyyyy
	// x = TableId(001~999) y = ElementId(00001~99999)
	UPROPERTY(EditDefaultsOnly)
	FString Id;
};