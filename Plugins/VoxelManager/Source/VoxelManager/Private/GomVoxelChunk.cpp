#include "GomVoxelChunk.h"
#include "GomProceduralMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "GomVoxelDataObject.h"
#include "GomVoxelDefinition.h"

AGomVoxelChunk::AGomVoxelChunk()
{
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
	MeshComponent = CreateDefaultSubobject<UGomProceduralMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->bAutoActivate = false;
}

void AGomVoxelChunk::InitChunk(UGomVoxelDefinition* InVoxelDefinition)
{
	if (InVoxelDefinition)
	{
		VoxelRange = InVoxelDefinition->VoxelRange;
		BlockSize = InVoxelDefinition->BlockSize;
		Padding = InVoxelDefinition->Padding;
		VoxelDataClass = InVoxelDefinition->VoxelDataClass;

		MeshComponent->SetMaterial(0, InVoxelDefinition->Material);
	}
}

void AGomVoxelChunk::GenerateVoxel()
{
	if (!VoxelDataClass)
	{
		VoxelDataClass = UGomVoxelDataObject::StaticClass();
	}
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
		FMath::FloorToFloat(Destination.Z) };

	DeleteVoxelBox(Destination);
}

void AGomVoxelChunk::DeactivateChunk()
{
	Multicast_DeactivateChunk();
}

void AGomVoxelChunk::ActivateChunk()
{
	Multicast_ActivateChunk();
}

void AGomVoxelChunk::InitializeVoxelData()
{
	VoxelWrapper.VoxelDataMap.Empty();
	TMap<FVector, FVoxelData> Tmp;
	for (float X = 0; X < VoxelRange; X++)
	{
		for (float Y = 0; Y < VoxelRange; Y++)
		{
			for (float Z = 0; Z < VoxelRange; Z++)
			{
				FVector Coord = FVector(X, Y, Z);
				UGomVoxelDataObject* DataObject = NewObject<UGomVoxelDataObject>(this, VoxelDataClass);
				DataObject->Coord = Coord;
				Tmp.Emplace(Coord, FVoxelData{ Coord, 0, DataObject});
			}
		}
	}

	VoxelWrapper.VoxelDataMap = MoveTemp(Tmp);

	for (const auto& Iter : VoxelWrapper.VoxelDataMap)
	{
		if (Iter.Value.VoxelDataObject)
		{
			Iter.Value.VoxelDataObject->OnInitVoxelDataObject(VoxelWrapper.VoxelDataMap);
		}
	}
}

void AGomVoxelChunk::GenerateChunkMesh()
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UVs;

	for (auto& Elem : VoxelWrapper.VoxelDataMap)
	{
		FVector VoxelCoord = Elem.Key;
		int32 VoxelType = Elem.Value.VoxelType;

		if (VoxelType == INDEX_NONE)
		{
			continue;
		}
		//AddVoxelMesh(Vertices, Triangles, UVs, VoxelCoord, VoxelType);
	}
	ScanZ();

	MeshComponent->CreateMeshSection_LinearColor(0, _Vertices, _Triangles, TArray<FVector>(), _UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);

	_Vertices.Empty();
	_Triangles.Empty();
	_UVs.Empty();
	//if (IsRunningGame())
	//{
	//	MeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	//}
	//else
	//{
	//	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//	MeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), false);
	//}
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
	if (VoxelWrapper.VoxelDataMap.Find(Coord))
	{
		return true;
	}

	return false;
}

void AGomVoxelChunk::DeleteVoxelBox(const FVector& Coord)
{
	FVoxelData* InternalData = VoxelWrapper.VoxelDataMap.Find(Coord);
	if (InternalData)
	{
		if (InternalData->VoxelDataObject)
		{
			InternalData->VoxelDataObject->Delegate_ExecuteDeleteVoxel.AddLambda([this, InternalData, Coord]() -> void
				{
					Multicast_OnDeleteVoxel(Coord);
				});
			InternalData->VoxelDataObject->OnReadyDeleteDataObject();
		}
		else
		{
			Multicast_OnDeleteVoxel(Coord);
		}
	}
}

void AGomVoxelChunk::Multicast_OnDeleteVoxel_Implementation(const FVector& Coord)
{
	FVoxelData* InternalData = VoxelWrapper.VoxelDataMap.Find(Coord);
	if (InternalData)
	{
		if (InternalData->VoxelDataObject)
		{
			InternalData->VoxelDataObject->OnDeleteVoxelDataObject();
		}
		VoxelWrapper.VoxelDataMap.Remove(InternalData->VoxelCoord);
		GenerateChunkMesh();
	}
}

void AGomVoxelChunk::Multicast_DeactivateChunk_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	MeshComponent->Deactivate();
}

void AGomVoxelChunk::Multicast_ActivateChunk_Implementation()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	MeshComponent->Activate();
}

void AGomVoxelChunk::ScanZ()
{
	for (int32 i = 0; i < VoxelRange; i++)
	{
		ScanTopFaceSlice(i, 1, FVector::UpVector);
		ScanTopFaceSlice(i, -1, FVector::DownVector);
		ScanTopFaceSlice(i, 1, FVector::RightVector);
		ScanTopFaceSlice(i, -1, FVector::LeftVector);
		ScanTopFaceSlice(i, 1, FVector::ForwardVector);
		ScanTopFaceSlice(i, -1, FVector::BackwardVector);
	}
}

void AGomVoxelChunk::ScanTopFaceSlice(int32 i, int32 Symbol, FVector Dir)
{
	TArray<bool>	Processed;
	TArray<bool>	IsVisible;
	TArray<int32>	BlockType;
	Processed.Init(false, VoxelRange * VoxelRange);
	IsVisible.Init(false, VoxelRange * VoxelRange);
	BlockType.Init(0, VoxelRange * VoxelRange);

	auto CoordGenerator = [this](float x, float y, float i, int32 Symbol, FVector Dir)->FVector
		{
			if (Dir == FVector::UpVector || Dir == FVector::DownVector)
			{
				return FVector { x, y, i + Symbol };
			}
			else if (Dir == FVector::RightVector || Dir == FVector::LeftVector)
			{
				return FVector{ x, i + Symbol, y };
			}
			else
			{
				return FVector{ i + Symbol, x, y };
			}
		};

	for (int32 y = 0; y < VoxelRange; y++)
	{
		for (int32 x = 0; x < VoxelRange; x++)
		{
			FVector CurrentCoord = CoordGenerator(x, y, i, 0, Dir);
			FVector NextCoord = CoordGenerator(x, y, i, Symbol, Dir);
			const FVoxelData* CurrentData = VoxelWrapper.VoxelDataMap.Find(CurrentCoord);
			const FVoxelData* NextData = VoxelWrapper.VoxelDataMap.Find(NextCoord);

			bool IsSolid = CurrentData && CurrentData->VoxelType != INDEX_NONE;
			bool IsNextSolid = NextData && NextData->VoxelType != INDEX_NONE;
			int32 Index = Get2DIndex(x, y, VoxelRange);
			IsVisible[Index] = IsSolid && !IsNextSolid;
			BlockType[Index] = IsSolid ? CurrentData->VoxelType : INDEX_NONE;
			Processed[Index] = false;
		}
	}

	GreedyMeshScan(Processed, IsVisible, BlockType, i, Dir, Symbol);
}

void AGomVoxelChunk::GreedyMeshScan(TArray<bool>& Processed, TArray<bool>& IsVisible, TArray<int32>& BlockType, int32 i, FVector Dir, int32 Symbol)
{
	auto CoordGenerator = [this](float x, float y, float i, FVector Dir)->FVector
		{
			if (Dir == FVector::UpVector || Dir == FVector::DownVector)
			{
				return FVector{ x, y, i };
			}
			else if (Dir == FVector::RightVector || Dir == FVector::LeftVector)
			{
				return FVector{ x, i, y };
			}
			else
			{
				return FVector{ i, x, y };
			}
		};
	for (int32 y = 0; y < VoxelRange; y++)
	{
		for (int32 x = 0; x < VoxelRange; x++)
		{
			int32 Index = Get2DIndex(x, y, VoxelRange);
			if (Processed[Index])
			{
				continue;
			}
			if (!IsVisible[Index])
			{
				continue;
			}

			int32 Type = BlockType[Index];

			// width 확장
			int32 Width = 1;
			while ((x + Width) < VoxelRange)
			{
				Index = Get2DIndex(x + Width, y, VoxelRange);
				bool bIsValid = !Processed[Index] &&
								IsVisible[Index] &&
								BlockType[Index] == Type;
				if (!bIsValid)
				{
					break;
				}
				++Width;
			}

			// height 확장
			int32 Height = 1;
			while ((y + Height) < VoxelRange)
			{
				bool valid = true;
				for (int dy = 0; dy < Height; dy++)
				{
					Index = Get2DIndex(x, y + Height, VoxelRange);
					if (Processed[Index] ||
						!IsVisible[Index]/* ||
						BlockType[Index] != Type*/)
					{
						valid = false;
						break;
					}
				}

				if (!valid)
				{
					break;
				}
				++Height;
			}

			// 쿼드 생성
			FVector Origin = CoordGenerator(x, y, i + (Symbol < 0 ? 0 : Symbol), Dir) * (BlockSize + Padding);
			FVector Size = FVector(Width, Height, 1) * (BlockSize + Padding);
			AddQuad(Origin, Size, Type, Dir);

			// 처리 마킹
			for (int dy = 0; dy < Height; dy++)
			{
				for (int dx = 0; dx < Width; dx++)
				{
					Index = Get2DIndex(x + dx, y + dy, VoxelRange);
					Processed[Index] = true;
				}
			}
		}
	}
}

void AGomVoxelChunk::AddQuad(const FVector& Origin, const FVector& Size, int32 VoxelType, const FVector& NormalDirection)
{
	FVector Tangent, Bitangent;
	if (NormalDirection == FVector::UpVector || NormalDirection == FVector::DownVector)
	{
		Tangent = FVector::ForwardVector;
		Bitangent = FVector::RightVector;
	}
	else if (NormalDirection == FVector::ForwardVector || NormalDirection == FVector::BackwardVector)
	{
		Tangent = FVector::RightVector;
		Bitangent = FVector::UpVector;
	}
	else
	{
		Tangent = FVector::UpVector;
		Bitangent = FVector::ForwardVector;
	}

	FVector V0 = Origin;
	FVector V1 = Origin + Bitangent * Size.Y;
	FVector V2 = Origin + Tangent * Size.X + Bitangent * Size.Y;
	FVector V3 = Origin + Tangent * Size.X;

	const float TileSize = 1.0f / 16.0f;
	int32 X = VoxelType % 16;
	int32 Y = VoxelType / 16;
	FVector2D UVOffset = FVector2D(X * TileSize, Y * TileSize);
	FVector2D UV0 = UVOffset + FVector2D(0, TileSize);         // 좌상단
	FVector2D UV1 = UVOffset + FVector2D(TileSize, TileSize);  // 우상단
	FVector2D UV2 = UVOffset + FVector2D(TileSize, 0);         // 우하단
	FVector2D UV3 = UVOffset + FVector2D(0, 0);                // 좌하단

	int32 StartIndex = _Vertices.Num();
	if (NormalDirection == FVector::UpVector || NormalDirection == FVector::RightVector)
	{
		_Vertices.Add(V0);
		_Vertices.Add(V1);
		_Vertices.Add(V2);
		_Vertices.Add(V3);
	}
	else if (NormalDirection == FVector::ForwardVector)
	{
		_Vertices.Add(V3);
		_Vertices.Add(V0);
		_Vertices.Add(V1);
		_Vertices.Add(V2);
	}
	else if (NormalDirection == FVector::BackwardVector)
	{
		_Vertices.Add(V0);
		_Vertices.Add(V3);
		_Vertices.Add(V2);
		_Vertices.Add(V1);
	}
	else if (NormalDirection == FVector::LeftVector)
	{
		_Vertices.Add(V1);
		_Vertices.Add(V0);
		_Vertices.Add(V3);
		_Vertices.Add(V2);
	}
	else
	{
		_Vertices.Add(V3);
		_Vertices.Add(V2);
		_Vertices.Add(V1);
		_Vertices.Add(V0);
	}

	_UVs.Add(UV0);
	_UVs.Add(UV1);
	_UVs.Add(UV2);
	_UVs.Add(UV3);

	_Triangles.Append({	StartIndex + 0, StartIndex + 1, StartIndex + 2,
						StartIndex + 0, StartIndex + 2, StartIndex + 3 });
}

int32 AGomVoxelChunk::Get2DIndex(int32 X, int32 Y, int32 Width)
{
	return X + Y * Width;
}
