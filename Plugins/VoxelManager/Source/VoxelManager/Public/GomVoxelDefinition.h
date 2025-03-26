// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/SubclassOf.h"
#include "GomVoxelDefinition.generated.h"

class UGomVoxelComponent;
class UMaterialInterface;
class UGomVoxelDataObject;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class VOXELMANAGER_API UGomVoxelDefinition : public UObject
{
	GENERATED_BODY()
/*
* Overrided Functions
*/
public:
	UGomVoxelDefinition();

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "GomVoxelProperty")
	TArray<TObjectPtr<UGomVoxelComponent>> VoxelComponents;

	// X * X * X
	UPROPERTY(EditAnywhere, Category="GomVoxelProperty")
	int32 ChunkRange = 4;

	UPROPERTY(EditAnywhere, Category="GomVoxelProperty")
	int32 VoxelRange = 4;

	UPROPERTY(EditAnywhere, Category = "GomVoxelProperty")
	int32 BlockSize = 30;
	
	UPROPERTY(EditAnywhere, Category = "GomVoxelProperty")
	int32 Padding = 0;

	UPROPERTY(EditAnywhere, Category = "GomVoxelProperty")
	float Distance = 5;

	UPROPERTY(EditAnywhere, Category = "GomVoxelProperty")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, Category = "GomVoxelProperty")
	TSubclassOf<UGomVoxelDataObject> VoxelDataClass;
};
