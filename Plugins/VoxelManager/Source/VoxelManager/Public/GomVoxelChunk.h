// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GomVoxelChunk.generated.h"

class UGomProceduralMeshComponent;

UENUM(BlueprintType)
enum class EVoxelType : uint8
{
	None UMETA(DisplayName = None),
	Rock UMETA(DisplayName = Rock),
};

struct FVertextData
{
	FVertextData(const FVector& Coord)
	: Vertex(Coord)
	, ReferenceCount(1)
	{}

	FVector Vertex;
	uint8	ReferenceCount;
};

USTRUCT()
struct FVoxelData
{
	GENERATED_BODY()

	FVoxelData() {}
	FVoxelData(const FVector& InVoxelCoord, EVoxelType InVoxelType)
	: VoxelType(InVoxelType)
	, VoxelCoord(InVoxelCoord)
	{}

	EVoxelType				VoxelType = EVoxelType::None;
	FVector					VoxelCoord;
	TArray<FVertextData>	VertexData;
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
	virtual void PostActorCreated() override;
	virtual void Destroyed() override;

protected:
	void InitializeVoxelData();
	void GenerateChunkMesh() const;
	void AddVoxelMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector2D>& UVs, int32 VoxelSize, const FVector& VoxelCoord) const;
/*
* Member Functions
*/
public:

/*
* Member Variables
*/
protected:
	UPROPERTY(VisibleAnywhere, Category="Voxel")
    UGomProceduralMeshComponent* MeshComponent;

	TMap<FVector, FVoxelData> VoxelData;
};
