#include "MTable_TableBase.h"

TMap<int32/*table id*/, FMTable_TableBase*> FMTable_TableBase::TableMap;

void FMTable_TableBase::OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems)
{
	Super::OnPostDataImport(InDataTable, InRowName, OutCollectedImportProblems);

	if (IsValidId())
	{
		int32 Key = GetKey();

		TableMap[Key] = this;
	}
}

bool FMTable_TableBase::IsValidId() const
{
	bool bIsValid = !Id.IsEmpty();

	FString TableId = Id.Left(3);
	bIsValid &= TableId.Len() == 3 && TableId.IsNumeric() && (FCString::Atoi(*TableId) >= 1 && FCString::Atoi(*TableId) <= 999);

	FString ElementId = Id.Right(5);
	bIsValid &= ElementId.Len() == 5 && ElementId.IsNumeric() && (FCString::Atoi(*ElementId) >= 1 && FCString::Atoi(*ElementId) <= 99999);

	return bIsValid;
}

int32 FMTable_TableBase::GetKey() const
{
	FString TableId = Id.Left(3);
	
	return FCString::Atoi(*TableId);
}

int32 FMTable_TableBase::GetElementId() const
{
	FString ElementId = Id.Right(5);

	return FCString::Atoi(*ElementId);
}
