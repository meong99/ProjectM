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
	//TArray<FVector> Vertices;
 //   TArray<int32> Triangles;
 //   TArray<FVector2D> UVs;

 //   float Size = 100.0f;  // 정육면체 한 변의 길이

 //   // 정점(Vertex) 설정 (8개의 꼭짓점)
 //   

 //   // 꼭짓점을 배열에 추가
 //   Vertices.Append({V0, V1, V2, V3, V4, V5, V6, V7});

 //   // 🌟 삼각형 인덱스(Triangle) 설정 (시계 반대 방향 CCW으로 감기 수정)
 //   // 앞면
 //   Triangles.Append({4, 6, 5, 4, 7, 6});
 //   // 뒷면
 //   Triangles.Append({0, 1, 2, 0, 2, 3});
 //   // 왼쪽 면
 //   Triangles.Append({0, 7, 4, 0, 3, 7});
 //   // 오른쪽 면
 //   Triangles.Append({1, 5, 6, 1, 6, 2});
 //   // 윗면
 //   Triangles.Append({3, 2, 6, 3, 6, 7});
 //   // 아랫면
 //   Triangles.Append({0, 4, 5, 0, 5, 1});

 //   // UV 좌표 설정 (각 면에 맞게 설정)
 //   UVs.Append({
 //       FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),  // 앞면
 //       FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),  // 뒷면
 //       FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),  // 왼쪽 면
 //       FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),  // 오른쪽 면
 //       FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1),  // 윗면
 //       FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1)   // 아랫면
 //   });


	//MeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	InitializeVoxelData();
	GenerateChunkMesh();
	MCHAE_LOG("AGomVoxelChunk::PostActorCreated");
}


void AGomVoxelChunk::Destroyed()
{
	Super::Destroyed();
}

void AGomVoxelChunk::InitializeVoxelData()
{
	int32 ChunkSize = 4;  // 🌟 4x4x4 크기의 복셀 청크

	for (float X = -(ChunkSize / 2) - 1.5; X < ChunkSize; X++)
	{
		for (float Y = -(ChunkSize / 2) - 1.5; Y < ChunkSize; Y++)
		{
			for (float Z = -(ChunkSize / 2) - 1.5; Z < ChunkSize; Z++)
			{
				FVector Coord = FVector(X, Y, Z);
				VoxelData.Emplace(Coord, FVoxelData{ Coord, EVoxelType::Rock });  // 🌟 기본 블록 타입 (1=돌)
			}
		}
	}
}

void AGomVoxelChunk::GenerateChunkMesh() const
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UVs;

	int32 VoxelSize = 30;  // 🌟 복셀 크기 (100cm)
	int32 ChunkSize = 4;    // 🌟 4x4x4 크기의 복셀 청크

	for (auto& Elem : VoxelData)
	{
		FVector VoxelCoord = Elem.Key;
		EVoxelType VoxelType = Elem.Value.VoxelType;

		if (VoxelType == EVoxelType::None) continue; // 공기 블록은 그리지 않음

		// 🌟 여기에 AddVoxelMesh() 함수를 호출해서 복셀 추가
		AddVoxelMesh(Vertices, Triangles, UVs, VoxelSize, VoxelCoord);
		break;
	}

	// 🌟 Procedural Mesh 생성
	MeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
}

void AGomVoxelChunk::AddVoxelMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector2D>& UVs, int32 VoxelSize, const FVector& VoxelCoord) const
{
	FVector CubeVertices[8] =
	{
		FVector(-1, -1, -1), // 후좌하
		FVector(1, -1, -1), // 전좌하
		FVector(1, 1, -1), // 전우하
		FVector(-1, 1, -1), // 후우하
		FVector(-1, -1, 1), // 후좌상
		FVector(1, -1, 1), //전좌상
		FVector(1, 1, 1), // 전우상
		FVector(-1, 1, 1) //후우상
	};

	int32 CubeTriangles[36] =
	{
		4, 6, 5, 4, 7, 6,  // 상
		0, 1, 2, 0, 2, 3,  // 하
		0, 1, 2, 0, 2, 3,  // 하
		0, 4, 5, 0, 5, 1,  // 좌
		2, 6, 7, 2, 7, 3,  // 우
		1, 5, 6, 1, 6, 2,   // 앞
		0, 3, 7, 0, 7, 4  // 뒤
	};

	FVector2D FaceUVs[4] =
	{
		FVector2D(0, 0),
		FVector2D(1, 0),
		FVector2D(1, 1),
		FVector2D(0, 1)
	};

	FVector faceChecks[6] = 
	{
		FVector(+1.0f,  0.0f,  0.0f),	// Front
		FVector(-1.0f,  0.0f,  0.0f),	// Back
		FVector(0.0f, +1.0f,  0.0f),	// Right
		FVector(0.0f, -1.0f,  0.0f),	// Left
		FVector(0.0f,  0.0f, +1.0f),	//Up
		FVector(0.0f,  0.0f, -1.0f),	//Down
	};

	int32 StartIndex = Vertices.Num();
	for (int i = 0; i < 8; i++)
	{
		Vertices.Add((VoxelCoord * (VoxelSize)) + (CubeVertices[i] * VoxelSize));  // 정점 추가
	}

	for (int i = asd; i < 36; i++)
	{
		Triangles.Add(StartIndex + CubeTriangles[i]);  // 삼각형 추가
	}

	for (int i = 0; i < 6; i++)  // 6개의 면
	{
		UVs.Append({ FaceUVs[0], FaceUVs[1], FaceUVs[2], FaceUVs[0], FaceUVs[2], FaceUVs[3] });
	}
}

