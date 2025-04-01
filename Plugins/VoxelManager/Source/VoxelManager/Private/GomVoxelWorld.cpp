#include "GomVoxelWorld.h"
#include "GomVoxelComponent.h"
#include "GomVoxelDefinition.h"
#include "GomVoxelChunk.h"
#include "Net/UnrealNetwork.h"

AGomVoxelWorld::AGomVoxelWorld()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
}

void AGomVoxelWorld::PostLoad()
{
	Super::PostLoad();
	if (ChunkPool.IsEmpty() && DeactivatedVoxelChunks.IsEmpty())
	{
		CreateChunkPool();
	}
	CachingChunkDefinition(VoxelDefinition);
	ActivateVoxel();
}

void AGomVoxelWorld::PostActorCreated()
{
	Super::PostActorCreated();
}

void AGomVoxelWorld::BeginDestroy()
{
	/*if (HasAuthority())
	{
		DeleteVoxel();
	}*/
	Super::BeginDestroy();
}

void AGomVoxelWorld::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.GetPropertyName();

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AGomVoxelWorld, VoxelDefinition) && VoxelDefinition)
	{
		CachingChunkDefinition(VoxelDefinition);
		GenerateVoxelFromLocation(GetActorLocation());
	}
}

void AGomVoxelWorld::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//ActivateVoxel();
}

void AGomVoxelWorld::DeactivateVoxel()
{
	for (auto Iter = DeactivatedVoxelChunks.CreateIterator(); Iter; ++Iter)
	{
		Iter->Value.Chunk->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Iter->Value.Chunk->SetActorLocation(FVector{ 0 });
		DeactivateChunk(Iter->Value.Chunk);
		ChunkPool.Add(Iter->Value);
		Iter.RemoveCurrent();
	}
}

void AGomVoxelWorld::ActivateVoxel()
{
	if (DeactivatedVoxelChunks.IsEmpty())
	{
		GenerateVoxelFromLocation(GetActorLocation());
	}
	for (auto& Iter : DeactivatedVoxelChunks)
	{
		Iter.Value.Chunk->InitChunk(VoxelDefinition);
		Iter.Value.Chunk->GenerateVoxel();
	}
}

void AGomVoxelWorld::CreateChunkPool()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		MCHAE_WARNING("World is not valid");
		return;
	}

	int32 CurrnetChunkNum = ChunkPoolSize - ChunkPool.Num() + DeactivatedVoxelChunks.Num() + ActivatedVoxelChunks.Num();
	for (int32 i = 0; i < CurrnetChunkNum; i++)
	{
		AGomVoxelChunk* NewChunk = World->SpawnActorDeferred<AGomVoxelChunk>(AGomVoxelChunk::StaticClass(), FTransform::Identity, this);
		DeactivateChunk(NewChunk);
		NewChunk->FinishSpawning(FTransform::Identity);
		
		FMChunkWrapper Wrapper;
		Wrapper.Chunk = NewChunk;
		ChunkPool.Add(Wrapper);
	}
}

void AGomVoxelWorld::UpdateVoxelFromLocation(const FVector& Location)
{
	bool bIsFirst = ActivatedVoxelChunks.IsEmpty();

	const TSet<FVector>& ChunksToActivate = GetChunkCoordsToActivate(Location);
	for (const FVector& CoordToActivate : ChunksToActivate)
	{
		FMChunkWrapper ChunkWrapper = DeactivatedVoxelChunks.FindRef(CoordToActivate);
		if (ChunkWrapper.Chunk)
		{
			ChunkWrapper.Chunk->ActivateChunk();
			ActivatedVoxelChunks.Add(CoordToActivate, ChunkWrapper);
			DeactivatedVoxelChunks.Remove(CoordToActivate);
		}

		if (!bIsFirst)
		{
			FVector CoordToDeactivate = CoordToActivate * -1;
			ChunkWrapper = ActivatedVoxelChunks.FindRef(CoordToDeactivate);
			if (ChunkWrapper.Chunk)
			{
				ChunkWrapper.Chunk->DeactivateChunk();
				DeactivatedVoxelChunks.Add(CoordToDeactivate, ChunkWrapper);
				ActivatedVoxelChunks.Remove(CoordToDeactivate);
			}
		}
	}
}

void AGomVoxelWorld::GenerateVoxelFromLocation(const FVector& Location)
{
	DeactivateVoxel();
	TSet<FVector> ChunksToGenerate = GetChunkCoordsToGenerate(Location);
	for (const FVector& Loc : ChunksToGenerate)
	{
		CreateChunk(Loc);
	}

	for (auto& Iter : DeactivatedVoxelChunks)
	{
		Iter.Value.Chunk->GenerateVoxel();
		ActivateChunk(Iter.Value.Chunk);
	}
}

TSet<FVector> AGomVoxelWorld::GetChunkCoordsToActivate(const FVector& WorldLocation) const
{
	TSet<FVector> ChunksToActivate;

	const FVector& MinBound = ChangeToVoxelCoord(WorldLocation - FVector(Distance));
	const FVector& MaxBound = ChangeToVoxelCoord(WorldLocation + FVector(Distance));

	for (int32 X = MinBound.X; X < MaxBound.X; X++)
	{
		for (int32 Y = MinBound.Y; Y < MaxBound.Y; Y++)
		{
			for (int32 Z = MinBound.Z; Z < MaxBound.Z; Z++)
			{
				FVector NewCoord = FVector(X, Y, Z);
				if (ActivatedVoxelChunks.Contains(NewCoord))
				{
					ChunksToActivate.Add(NewCoord);
				}
			}
		}
	}

	return ChunksToActivate;
}

TSet<FVector> AGomVoxelWorld::GetChunkCoordsToGenerate(const FVector& WorldLocation) const
{
	TSet<FVector> ChunksToGenerate;

	const FVector& MinBound = FVector{ 0 };
	const FVector& MaxBound = FVector{ (float)ChunkRange };

	for (int32 X = MinBound.X; X < MaxBound.X; X++)
	{
		for (int32 Y = MinBound.Y; Y < MaxBound.Y; Y++)
		{
			//for (int32 Z = MinBound.Z; Z < MaxBound.Z; Z++)
			{
				FVector NewCoord = FVector(X, Y, 0);

				ChunksToGenerate.Add(NewCoord);
			}
		}
	}

	return ChunksToGenerate;
}

FVector AGomVoxelWorld::ChangeToVoxelCoord(const FVector& Location) const
{
	int32 ChunkDist = BlockSize * VoxelRange;
	FVector VoxelBaseLocation = GetActorLocation();
	FVector TransCoord = (Location - VoxelBaseLocation) / ChunkDist;
	FVector Coord{ FMath::Floor(TransCoord.X), FMath::Floor(TransCoord.Y), FMath::Floor(TransCoord.Z) };

	return Coord;
}

FVector AGomVoxelWorld::ChangeToWorldLocation(const FVector& Coord) const
{
	int32 ChunkDist = BlockSize * VoxelRange;
	FVector VoxelBaseLocation = GetActorLocation();
	FVector TransLocation = (Coord * ChunkDist) + VoxelBaseLocation;

	return TransLocation;
}

void AGomVoxelWorld::CreateChunk(const FVector& NewChunkCoord)
{
	UWorld* World = GetWorld();

	if (World)
	{
		FVector Loc = ChangeToWorldLocation(NewChunkCoord);

		FActorSpawnParameters Params;
		Params.Owner = this;

		FMChunkWrapper NewChunk = ChunkPool[ChunkPool.Num() - 1];
		ChunkPool.RemoveAt(ChunkPool.Num() - 1);
		if (!NewChunk.Chunk)
		{
			MCHAE_WARNING("ChunkPool is empty");
			return;
		}
		NewChunk.Chunk->InitChunk(VoxelDefinition);
		NewChunk.Chunk->SetActorLocation(Loc);
		NewChunk.Chunk->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		DeactivatedVoxelChunks.Add(NewChunkCoord, NewChunk);
	}
}

void AGomVoxelWorld::CachingChunkDefinition(const UGomVoxelDefinition* NewVoxelDefinition)
{
	if (NewVoxelDefinition)
	{
		ChunkRange = NewVoxelDefinition->ChunkRange;
		VoxelRange = NewVoxelDefinition->VoxelRange;
		BlockSize = NewVoxelDefinition->BlockSize;
		Padding = NewVoxelDefinition->Padding;
		Distance = NewVoxelDefinition->Distance;
		VoxelDataClass = NewVoxelDefinition->VoxelDataClass;
	}
}

void AGomVoxelWorld::DeactivateChunk(AGomVoxelChunk* Chunk)
{
	if (Chunk)
	{
		Chunk->DeactivateChunk();
	}
}

void AGomVoxelWorld::ActivateChunk(AGomVoxelChunk* Chunk)
{
	if (Chunk)
	{
		Chunk->ActivateChunk();
	}
}
