// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GomVoxelWorld.generated.h"

class UGomVoxelDefinition;
class UGomVoxelComponent;
class AGomVoxelChunk;

UCLASS()
class VOXELMANAGER_API AGomVoxelWorld : public AActor
{
	GENERATED_BODY()
	
/*
* Overrided Functions
*/
public:
	AGomVoxelWorld();
	virtual void PostInitializeComponents() override;
	virtual void BeginDestroy() override;

/*
* Member Functions
*/
public:
	UFUNCTION(CallInEditor, Category = "GomVoxelProperty")
	void GenerateVoxel();
	UFUNCTION(CallInEditor, Category = "GomVoxelProperty")
	void DeleteVoxel();
	UFUNCTION(CallInEditor, Category = "GomVoxelProperty")
	void RegenerateVoxel();

protected:
	void CreateNewChunk();
/*
* Member Variables
*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "GomVoxelProperty")
	TObjectPtr<UGomVoxelDefinition> VoxelDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "GomVoxelProperty")
	TArray<TObjectPtr<UGomVoxelComponent>> VoxelComponents;

#pragma TODO("이거 Fast로 바꿔야함")
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "GomVoxelProperty")
	TArray<TObjectPtr<AGomVoxelChunk>> VoxelChunks;
};
