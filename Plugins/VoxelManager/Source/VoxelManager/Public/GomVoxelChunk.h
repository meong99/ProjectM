// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GomVoxelChunk.generated.h"

class UGomProceduralMeshComponent;
class UGomVoxelDataObject;
class UMaterialInterface;
class UGomVoxelDefinition;

USTRUCT()
struct FVoxelData
{
	GENERATED_BODY()

	FVoxelData()
	: VoxelType(INDEX_NONE)
	{}
	FVoxelData(const FVector& InVoxelCoord, int32 InVoxelType, UGomVoxelDataObject* InVoxelObject)
	: VoxelType(InVoxelType)
	, VoxelCoord(InVoxelCoord)
	, VoxelDataObject(InVoxelObject)
	{}

	UPROPERTY()
	int32							VoxelType;

	UPROPERTY()
	FVector							VoxelCoord;

	UPROPERTY()
	TObjectPtr<UGomVoxelDataObject>	VoxelDataObject;
};

USTRUCT()
struct FVoxelWrapper
{
	GENERATED_BODY()

	FVoxelWrapper()
	{}

	UPROPERTY()
	TMap<FVector, FVoxelData> VoxelDataMap;
};

UCLASS()
class VOXELMANAGER_API AGomVoxelChunk : public AActor
{
	GENERATED_BODY()
	
/*
* Overrided Functions
*/
public:
	AGomVoxelChunk();

/*
* Member Functions
*/
public:
	void InitChunk(UGomVoxelDefinition* InVoxelDefinition);
	void GenerateVoxel();
	void HitVoxel(const FHitResult& HitResult);
	void DeactivateChunk();
	void ActivateChunk();

protected:
	void InitializeVoxelData();
	void GenerateChunkMesh();
	void AddVoxelMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector2D>& UVs, const FVector& VoxelCoord, int32 VoxelType) const;
	bool IsContactedFace(const FVector& Coord) const;
	void DeleteVoxelBox(const FVector& Coord);
	void DeleteVoxelBox_Internal(const FVector& Coord);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDeleteVoxel(const FVector& Coord);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DeactivateChunk();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ActivateChunk();

/*
* Member Variables
*/
protected:
	UPROPERTY(VisibleAnywhere, Category="GomVoxelProperty")
    UGomProceduralMeshComponent* MeshComponent;

	UPROPERTY()
	FVoxelWrapper VoxelWrapper;

	int32 VoxelRange;
	int32 BlockSize;
	int32 Padding;
	TSubclassOf<UGomVoxelDataObject> VoxelDataClass;

	void	ScanZ();
	void	ScanTopFaceSlice(int32 Z);
	void	GreedyMeshScan(TArray<bool>& Processed, TArray<bool>& IsVisible, TArray<int32>& BlockType, int32 Z);
	void	AddQuad(const FVector& Origin, const FVector& Size, int32 VoxelType, const FVector& NormalDirection);

	int32	Get2DIndex(int32 X, int32 Y, int32 Width);

	TArray<FVector>		_Vertices;
	TArray<int32>		_Triangles;
	TArray<FVector2D>	_UVs;
};
