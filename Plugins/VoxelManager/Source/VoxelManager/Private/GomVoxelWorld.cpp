#include "GomVoxelWorld.h"
#include "GomVoxelComponent.h"
#include "GomVoxelDefinition.h"
#include "GomVoxelChunk.h"

AGomVoxelWorld::AGomVoxelWorld()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGomVoxelWorld::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, VoxelChunks);
}


void AGomVoxelWorld::Destroyed()
{
	for (int32 i = 0; i < VoxelChunks.Num(); i++)
	{
		if (VoxelChunks[i])
		{
			VoxelChunks[i]->Destroy();
		}
	}

	VoxelChunks.Empty();
	Super::Destroyed();
}

void AGomVoxelWorld::GenerateVoxel()
{
	MCHAE_LOG("GenerateVoxel");
	CreateNewChunk();
}

void AGomVoxelWorld::DeleteVoxel()
{
	for (int32 i = 0; i < VoxelChunks.Num(); i++)
	{
		if (VoxelChunks[i])
		{
			VoxelChunks[i]->Destroy();
		}
	}

	VoxelChunks.Empty();
}

void AGomVoxelWorld::CreateNewChunk()
{
	UWorld* World = GetWorld();
	if (VoxelChunks.Num() != 0)
	{
		DeleteVoxel();
	}

	if (World)
	{
		FVector Loc = GetActorLocation();
		FActorSpawnParameters Params;
		Params.Owner = this;

		AGomVoxelChunk* NewChunk = GetWorld()->SpawnActor<AGomVoxelChunk>(AGomVoxelChunk::StaticClass(), GetActorTransform(), Params);
		NewChunk->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		VoxelChunks.Add(NewChunk);
	}
}
