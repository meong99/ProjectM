#pragma once

#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "MMonsterSpawner.generated.h"

class UMMonsterDefinition;
class AMMonsterBase;
class USplineComponent;
class UBillboardComponent;

UCLASS()
class PROJECTM_API AMMonsterSpawner : public AActor
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	AMMonsterSpawner(const FObjectInitializer& ObjectInitializer);
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds);

	/*
* Member Functions
*/
public:
	void OnDeadMonster(AMMonsterBase* DeadMonster);

protected:
	void ChangeMonsterDefinition();
	void SpawnMonster();

/*
* Member Variables
*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	FGameplayTag SearchTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	TObjectPtr<UBillboardComponent> BillboardComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	UMMonsterDefinition* MonsterDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster", meta = (ClampMin = 1400001, ClampMax = 1499999))
	int32 MonsterRowId = INDEX_NONE;

	UPROPERTY()
	TSet<AMMonsterBase*> SpawnedMonsters;

	float SpawnTimer = 0.0f;
};