#include "GomVoxelChunk.h"
#include "GomProceduralMeshComponent.h"
#include "ProceduralMeshComponent.h"

// Sets default values
AGomVoxelChunk::AGomVoxelChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UGomProceduralMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void AGomVoxelChunk::PostActorCreated()
{
	InitializeVoxelData();
	GenerateChunkMesh();
	MCHAE_LOG("AGomVoxelChunk::PostActorCreated");
}

void AGomVoxelChunk::Destroyed()
{
	Super::Destroyed();
}

void AGomVoxelChunk::RegenerateVoxel()
{
	InitializeVoxelData();
	GenerateChunkMesh();
}

void AGomVoxelChunk::InitializeVoxelData()
{
	VoxelData.Empty();
	int32 Half = (ChunkSize / 2);
	for (float X = -Half; X < Half; X++)
	{
		for (float Y = -Half; Y < Half; Y++)
		{
			for (float Z = -Half; Z < Half; Z++)
			{
				FVector Coord = FVector(X, Y, Z);
				VoxelData.Emplace(Coord, FVoxelData{ Coord, EVoxelType::Rock });
			}
		}
	}
}

void AGomVoxelChunk::GenerateChunkMesh() const
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UVs;

	for (auto& Elem : VoxelData)
	{
		FVector VoxelCoord = Elem.Key;
		EVoxelType VoxelType = Elem.Value.VoxelType;

		if (VoxelType == EVoxelType::None)
		{
			continue;
		}

		AddVoxelMesh(Vertices, Triangles, UVs, VoxelCoord);
	}

	MeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
}

void AGomVoxelChunk::AddVoxelMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector2D>& UVs, const FVector& VoxelCoord) const
{
	FVector CubeVertices[8] =
	{
		FVector(1,	0,	0),	//전좌하
		FVector(1,	0,	1),	//전좌상
		FVector(1,	1,	1),	//전우상
		FVector(1,	1,	0),	//전우하
		FVector(0,	0,	0),	//후좌하
		FVector(0,	0,	1),	//후좌상
		FVector(0,	1,	1),	//후우상
		FVector(0,	1,	0)	//후우하
	};

	int32 FaceVertex[6][4] =
	{
		{0, 1, 2, 3},	// 전
		{4, 7, 6, 5},	// 후
		{5, 6, 2, 1},	// 상
		{4, 0, 3, 7},	// 하
		{4, 5, 1, 0},	// 좌
		{3, 2, 6, 7}	// 우
	};

	FVector2D FaceUVs[4] =
	{
		FVector2D(0, 0),
		FVector2D(1, 0),
		FVector2D(1, 1),
		FVector2D(0, 1)
	};

	FVector FaceChecks[6] =
	{
		FVector(+1.0f,  0.0f,  0.0f),	// 전
		FVector(-1.0f,  0.0f,  0.0f),	// 후
		FVector(0.0f,  0.0f, +1.0f),	// 상
		FVector(0.0f,  0.0f, -1.0f),	// 하
		FVector(0.0f, -1.0f,  0.0f),	// 좌
		FVector(0.0f, +1.0f,  0.0f),	// 우
	};

	for (int i = 0; i < 6; i++)
	{
		if (IsContactedFace(VoxelCoord + FaceChecks[i]))
		{
			continue;
		}

		int32 StartIndex = Vertices.Num();
		for (int j = 0; j < 4; j++)
		{
			Vertices.Add(((VoxelCoord * (BlockSize + Padding))) + (CubeVertices[FaceVertex[i][j]] * BlockSize));
		}
		Triangles.Add(StartIndex + 0);
		Triangles.Add(StartIndex + 1);
		Triangles.Add(StartIndex + 2);
		Triangles.Add(StartIndex + 0);
		Triangles.Add(StartIndex + 2);
		Triangles.Add(StartIndex + 3);

		UVs.Append({ FaceUVs[0], FaceUVs[1], FaceUVs[2], FaceUVs[0], FaceUVs[2], FaceUVs[3] });
	}
}

bool AGomVoxelChunk::IsContactedFace(const FVector& Coord) const
{
	if (VoxelData.Find(Coord))
	{
		return true;
	}

	return false;
}

