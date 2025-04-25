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
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void InitCharacterName() override;

/*
* Member Functions
*/
public:
	UMNpcDefinition* GetNpcDefinition() const { return NpcDefinition; }
/*

* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "NPC")
	UMNpcDefinition* NpcDefinition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	UMInteractionComponent* InteractionComponent;
};
