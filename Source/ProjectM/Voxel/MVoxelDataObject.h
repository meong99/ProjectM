// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GomVoxelDataObject.h"
#include "MVoxelDataObject.generated.h"

/**
 *
 */
UCLASS()
class PROJECTM_API UMVoxelDataObject : public UGomVoxelDataObject
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	virtual void OnInitVoxelDataObject(TMap<FVector, FVoxelData>& VoxelData) override;
	virtual void OnReadyDeleteDataObject() override;
	virtual void OnDeleteVoxelDataObject() override;

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
protected:
};
