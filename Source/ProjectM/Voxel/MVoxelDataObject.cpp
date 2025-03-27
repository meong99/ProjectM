#include "MVoxelDataObject.h"

void UMVoxelDataObject::OnInitVoxelDataObject(TMap<FVector, FVoxelData>& VoxelData)
{
	for (auto& Iter : VoxelData)
	{
		//int r = rand() % 50;
		Iter.Value.VoxelType = 12;
	}
}

void UMVoxelDataObject::OnReadyDeleteDataObject()
{
	Delegate_ExecuteDeleteVoxel.Broadcast();
}

void UMVoxelDataObject::OnDeleteVoxelDataObject()
{
}
