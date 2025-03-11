// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Destroyed() override;

	/*
* Member Functions
*/
public:
	UFUNCTION(CallInEditor, Category = "VoxelGenerator")
	void GenerateVoxel();
	UFUNCTION(CallInEditor, Category = "VoxelGenerator")
	void DeleteVoxel();

protected:
	void CreateNewChunk();
/*
* Member Variables
*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TObjectPtr<UGomVoxelDefinition> VoxelDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<UGomVoxelComponent>> VoxelComponents;

#pragma TODO("이거 Fast로 바꿔야함")
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AGomVoxelChunk>> VoxelChunks;
};
