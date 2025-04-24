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
	virtual void PostLoad() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

/*
* Member Functions
*/
public:
	void ChangeMonsterDefinition();
/*
* Member Variables
*/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	const UMMonsterDefinition* MonsterDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster", meta = (ClampMin = 1400001, ClampMax = 1499999))
	int32 MonsterRowId = INDEX_NONE;
};