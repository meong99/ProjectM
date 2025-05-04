// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MCharacterBase.h"
#include "MNpcBase.generated.h"

class UMNpcDefinition;
class UMInteractionComponent;

/**
 * 
 */
UCLASS()
class PROJECTM_API AMNpcBase : public AMCharacterBase
{
	GENERATED_BODY()
/*
* Overrided Function
*/
public:
	AMNpcBase(const FObjectInitializer& ObjectInitializer);
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void InitCharacterName() override;

	/*
* Member Functions
*/
public:
	const UMNpcDefinition* GetNpcDefinition() const { return NpcDefinition; }
/*

* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=1500001, ClampMax = 1599999), Category = "ProjectM")
	int32 NpcRowId = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	TObjectPtr<UMNpcDefinition> NpcDefinition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	UMInteractionComponent* InteractionComponent;
};
