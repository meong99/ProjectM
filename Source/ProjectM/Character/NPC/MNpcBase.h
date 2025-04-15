// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MNpcBase.generated.h"

class UMNameWidgetComponent;
class UMNpcDefinition;
class UMInteractionComponent;
class USphereComponent;

/**
 * 
 */
UCLASS()
class PROJECTM_API AMNpcBase : public ACharacter
{
	GENERATED_BODY()
/*
* Overrided Function
*/
public:
	AMNpcBase();
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	/*
* Member Functions
*/
public:
	UMNpcDefinition* GetNpcDefinition() const { return NpcDefinition; }
	USphereComponent* GetInteractionShpere() const { return InteractionShpere; }

/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USphereComponent* InteractionShpere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	UMNameWidgetComponent* NpcNameComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "NPC")
	UMNpcDefinition* NpcDefinition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
	UMInteractionComponent* InteractionComponent;
};
