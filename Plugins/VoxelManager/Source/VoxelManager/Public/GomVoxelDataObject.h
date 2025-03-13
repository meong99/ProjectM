// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GomVoxelChunk.h"
#include "GomVoxelDataObject.generated.h"

/**
 * 
 */
UCLASS()
class VOXELMANAGER_API UGomVoxelDataObject : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void OnInitVoxelDataObject(TMap<FVector, FVoxelData>&) {};
	virtual void OnReadyDeleteDataObject() {};
	virtual void OnDeleteVoxelDataObject() {};

	// Broadcast를 해야 해당 오브젝트를 들고있는 복셀이 제거됨.
	FSimpleMulticastDelegate Delegate_ExecuteDeleteVoxel;

	FVector Coord;
};
