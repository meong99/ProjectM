#pragma once

#include "GameFramework/Actor.h"
#include "MMonsterSpawner.generated.h"

class UMMonsterDefinition;

UCLASS()
class PROJECTM_API AMMonsterSpawner : public AActor
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	AMMonsterSpawner(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

/*
* Member Functions
*/
public:
/*
* Member Variables
*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "Monster")
	UMMonsterDefinition* MonsterDefinition;
};