// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Containers/Queue.h"
#include "GomVoxelWorld.generated.h"

class UGomVoxelDefinition;
class UGomVoxelComponent;
class AGomVoxelChunk;
class UGomVoxelDataObject;

USTRUCT(BlueprintType)
struct FMChunkWrapper
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<AGomVoxelChunk> Chunk;
};

UCLASS()
class VOXELMANAGER_API AGomVoxelWorld : public AActor
{
	GENERATED_BODY()
	
/*
* Overrided Functions
*/
public:
	AGomVoxelWorld();
	virtual void PostLoad() override;
	virtual void PostActorCreated() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginDestroy() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

/*
* Member Functions
*/
public:
	UFUNCTION(CallInEditor, Category = "GomVoxelProperty")
	void DeactivateVoxel();
	UFUNCTION(CallInEditor, Category = "GomVoxelProperty")
	void ActivateVoxel();
	UFUNCTION(CallInEditor, Category = "GomVoxelProperty")
	void CreateChunkPool();

	void UpdateVoxelFromLocation(const FVector& Location);

protected:
	TSet<FVector> GetChunkCoordsToActivate(const FVector& WorldLocation) const;
	TSet<FVector> GetChunkCoordsToGenerate(const FVector& WorldLocation) const;
	FVector ChangeToVoxelCoord(const FVector& Location) const;
	FVector ChangeToWorldLocation(const FVector& Coord) const;
	void	GenerateVoxelFromLocation(const FVector& Location);
	void	CreateChunk(const FVector& NewChunkCoord);
	void	CachingChunkDefinition(const UGomVoxelDefinition* NewVoxelDefinition);
	void	DeactivateChunk(AGomVoxelChunk* Chunk);
	void	ActivateChunk(AGomVoxelChunk* Chunk);
/*
* Member Variables
*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "GomVoxelProperty")
	TObjectPtr<UGomVoxelDefinition> VoxelDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "GomVoxelProperty")
	TArray<TObjectPtr<UGomVoxelComponent>> VoxelComponents;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GomVoxelProperty")
	int32 ChunkPoolSize = 1000;

#pragma TODO("이거 Fast로 바꿔야함")
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GomVoxelProperty")
	TMap<FVector, FMChunkWrapper> DeactivatedVoxelChunks;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GomVoxelProperty")
	TMap<FVector, FMChunkWrapper> ActivatedVoxelChunks;

	// 캐싱
	int32 ChunkRange = 4;
	int32 VoxelRange = 4;
	int32 BlockSize = 30;
	int32 Padding = 0;
	float Distance = 5;
	TSubclassOf<UGomVoxelDataObject> VoxelDataClass;

	UPROPERTY(VisibleAnywhere)
	TArray<FMChunkWrapper> ChunkPool;
};
