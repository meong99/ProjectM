#include "MTable_TableBase.h"
#include "Definitions/MDefinitionBase.h"

void FMTable_TableBase::OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName)
{
#if WITH_EDITOR
	UMDefinitionBase* DefinitionCDO = Cast<UMDefinitionBase>(Definition.GetDefaultObject());
	if (DefinitionCDO)
	{
		DefinitionCDO->RowId = RowId;
	}
#endif
}

bool FMTable_TableBase::IsValidId() const
{
	bool bIsValid = RowId != INDEX_NONE;

	int32 TableId = GetKey();
	bIsValid &= TableId >= 1;

	int32 ElementId = GetElementId();
	bIsValid &= ElementId >= 1 && ElementId <= 99999;

	return bIsValid;
}

int32 FMTable_TableBase::GetKey() const
{
	int32 Temp = RowId / 100000;
	int32 TableId = INDEX_NONE;
	while (Temp)
	{
		TableId += Temp % 10;
		Temp /= 10;
	}

	return TableId;
}

int32 FMTable_TableBase::GetElementId() const
{
	int32 ElementId = INDEX_NONE;
	if (RowId / 100000)
	{
		ElementId = RowId % 100000;
	}

	return ElementId;
}
