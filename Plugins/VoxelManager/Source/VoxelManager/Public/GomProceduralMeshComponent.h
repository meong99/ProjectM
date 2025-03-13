// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GomProceduralMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class VOXELMANAGER_API UGomProceduralMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
/*
* Overrided Functions
*/
public:
	UGomProceduralMeshComponent(const FObjectInitializer& ObjectInitializer);
	virtual void InitializeComponent() override;

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
protected:
};
