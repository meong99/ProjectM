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

void AGomVoxelChunk::PostLoad()
{
	Super::PostLoad();
	if (VoxelData.IsEmpty())
	{
		RegenerateVoxel();
	}
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

void AGomVoxelChunk::HitVoxel(const FHitResult& HitResult)
{
	FVector HitLocation = HitResult.Location;
	FVector ChunkLocation = GetActorLocation();
	FVector Destination = ((HitLocation - ChunkLocation) / (BlockSize + Padding)) + (-HitResult.Normal * 0.5);

	Destination = FVector{
		FMath::FloorToFloat(Destination.X),
		FMath::FloorToFloat(Destination.Y),
		FMath::FloorToFloat(Destination.Z)};

	DeleteVoxelBox(Destination);
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
				VoxelData.Emplace(Coord, FVoxelData{ Coord, TestVoxelType });
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
		int32 VoxelType = Elem.Value.VoxelType;

		if (VoxelType == INDEX_NONE)
		{
			continue;
		}

		AddVoxelMesh(Vertices, Triangles, UVs, VoxelCoord, VoxelType);
	}

	MeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
}

void AGomVoxelChunk::AddVoxelMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector2D>& UVs, const FVector& VoxelCoord, int32 VoxelType) const
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
		{3, 0, 1, 2},	// 전
		{4, 7, 6, 5},	// 후
		{5, 6, 2, 1},	// 상
		{0, 3, 7, 4},	// 하
		{0, 4, 5, 1},	// 좌
		{7, 3, 2, 6}	// 우
	};

	/*FVector2D FaceUVs[4] =
	{
		FVector2D(0, 0),
		FVector2D(1, 1),
		FVector2D(0, 1),
		FVector2D(1, 0),
	};*/

	FVector FaceChecks[6] =
	{
		FVector(+1.0f,  0.0f,  0.0f),	// 전
		FVector(-1.0f,  0.0f,  0.0f),	// 후
		FVector(0.0f,  0.0f, +1.0f),	// 상
		FVector(0.0f,  0.0f, -1.0f),	// 하
		FVector(0.0f, -1.0f,  0.0f),	// 좌
		FVector(0.0f, +1.0f,  0.0f),	// 우
	};

	const float TileSize = 1.0f / 16.0f;  // 16x16 블록 크기
	int32 X = VoxelType % 16;
	int32 Y = VoxelType / 16;

	FVector2D UVOffset = FVector2D(X * TileSize, Y * TileSize);

	/*FVector2D FaceUVs[4] = {
		FVector2D(UVOffset.X, UVOffset.Y),
		FVector2D(UVOffset.X + TileSize, UVOffset.Y + TileSize),
		FVector2D(UVOffset.X, UVOffset.Y + TileSize),
		FVector2D(UVOffset.X + TileSize, UVOffset.Y)
	};*/
	FVector2D FaceUVs[4] = {
		UVOffset + FVector2D(0, TileSize),            // 좌상단 (0,1)
		UVOffset + FVector2D(TileSize, TileSize),      // 우상단 (1,1)
		UVOffset + FVector2D(TileSize, 0),            // 우하단 (1,0)
		UVOffset,                                      // 좌하단 (0,0)
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
			UVs.Add(FaceUVs[j]);
		}
		Triangles.Append({StartIndex + 0, StartIndex + 1, StartIndex + 2, StartIndex + 0, StartIndex + 2, StartIndex + 3});
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

void AGomVoxelChunk::DeleteVoxelBox(const FVector& Coord)
{
	if (VoxelData.Find(Coord))
	{
		VoxelData.Remove(Coord);
		GenerateChunkMesh();
	}
}

