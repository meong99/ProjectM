// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GomVoxelChunk.generated.h"

class UGomProceduralMeshComponent;
class UGomVoxelDataObject;

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

	int32							VoxelType;
	FVector							VoxelCoord;

	UPROPERTY()
	TObjectPtr<UGomVoxelDataObject>	VoxelDataObject;
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
	virtual void PostLoad() override;
	virtual void PostActorCreated() override;
	virtual void Destroyed() override;

/*
* Member Functions
*/
public:
	void RegenerateVoxel();
	void HitVoxel(const FHitResult& HitResult);

protected:
	void InitializeVoxelData();
	void GenerateChunkMesh() const;
	void AddVoxelMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector2D>& UVs, const FVector& VoxelCoord, int32 VoxelType) const;
	bool IsContactedFace(const FVector& Coord) const;
	void DeleteVoxelBox(const FVector& Coord);
	void DeleteVoxelBox_Internal(const FVector& Coord);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDeleteVoxel(const FVector& Coord);

/*
* Member Variables
*/
protected:
	UPROPERTY(VisibleAnywhere, Category="GomVoxelProperty")
    UGomProceduralMeshComponent* MeshComponent;

	// X * X * X
	UPROPERTY(EditAnywhere, Category="GomVoxelProperty")
	int32 ChunkSize = 4;

	UPROPERTY(EditAnywhere, Category = "GomVoxelProperty")
	int32 BlockSize = 30;

	UPROPERTY(EditAnywhere, Category = "GomVoxelProperty")
	int32 Padding = 0;

	UPROPERTY(EditAnywhere, Category = "GomVoxelProperty")
	int32 TestVoxelType = 0;

	UPROPERTY(EditAnywhere, Category = "GomVoxelProperty")
	TSubclassOf<UGomVoxelDataObject> VoxelDataClass;

	UPROPERTY()
	TMap<FVector, FVoxelData> VoxelData;
};
