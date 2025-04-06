// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MNpcBase.generated.h"

class UMNameWidgetComponent;
class UMNpcDefinition;

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

	/*
* Member Functions
*/
public:
	UMNpcDefinition* GetNpcDefinition() const { return NpcDefinition; }
/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, Category = "NPC")
	UMNameWidgetComponent* NpcNameComp;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "NPC")
	UMNpcDefinition* NpcDefinition;
};
