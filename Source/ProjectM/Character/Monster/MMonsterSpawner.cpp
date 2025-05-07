#include "MMonsterSpawner.h"
#include "Character/Monster/MMonsterTypes.h"
#include "Engine/Engine.h"
#include "Util/MGameplayStatics.h"
#include "System/MDataTableManager.h"
#include "Definitions/MMonsterDefinition.h"
#include "Engine/World.h"
#include "MMonsterBase.h"
#include "Components/SplineComponent.h"

AMMonsterSpawner::AMMonsterSpawner(const FObjectInitializer& ObjectInitializer)
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	bNetLoadOnClient = false;
	PrimaryActorTick.bCanEverTick = true;
}

#if WITH_EDITOR
void AMMonsterSpawner::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property->GetFName();
	if (GEngine && !HasAnyFlags(RF_ClassDefaultObject | RF_Transient) && \
		PropertyName == GET_MEMBER_NAME_CHECKED(AMMonsterSpawner, MonsterRowId) &&
		!IsRunningGame())
	{
		ChangeMonsterDefinition();
	}
}
#endif

void AMMonsterSpawner::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!HasAnyFlags(RF_ClassDefaultObject | RF_Transient) && !MonsterDefinition)
	{
		ChangeMonsterDefinition();
	}
}

void AMMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (MonsterDefinition)
	{
		for (int32 i = 0; i < MonsterDefinition->GetMaximumSpawnNum() - SpawnedMonsters.Num(); i++)
		{
			SpawnMonster();
		}
	}
}

void AMMonsterSpawner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (MonsterDefinition && SpawnedMonsters.Num() < MonsterDefinition->GetMaximumSpawnNum())
	{
		SpawnTimer += DeltaSeconds;
		if (SpawnTimer >= MonsterDefinition->GetMonsterSpawningCooldown())
		{
			SpawnMonster();
		}
	}
	else
	{
		SpawnTimer = 0.0f;
	}
}

void AMMonsterSpawner::OnDeadMonster(AMMonsterBase* DeadMonster)
{
	if (DeadMonster)
	{
		SpawnedMonsters.Remove(DeadMonster);
		DeadMonster->Destroy();
	}
}

void AMMonsterSpawner::ChangeMonsterDefinition()
{
	UMDataTableManager* TableManager = GEngine->GetEngineSubsystem<UMDataTableManager>();
	if (TableManager)
	{
		MonsterDefinition = TableManager->GetMonsterDefinition(MonsterRowId);
	}

	if (!MonsterDefinition && !IsRunningGame())
	{
		UMGameplayStatics::ShowErrorOrLog(TEXT("MonsterDefinition is not valid!MonsterSpawner must be set MonsterRawId!!!!!"));
	}
}

void AMMonsterSpawner::SpawnMonster()
{
	if (MonsterDefinition->GetMaximumSpawnNum() <= SpawnedMonsters.Num())
	{
		return;
	}
	AMMonsterBase* SpawnedMonster = GetWorld()->SpawnActorDeferred<AMMonsterBase>(MonsterDefinition->GetMonsterClass(), FTransform::Identity);
	if (SpawnedMonster)
	{
		SpawnedMonster->SetSpawner(this);

		FTransform Transform;
		int32 PointIndex = FMath::RandHelper(SplineComponent->GetNumberOfSplinePoints());
		const FSplinePoint& Point = SplineComponent->GetSplinePointAt(PointIndex, ESplineCoordinateSpace::World);

		FVector Location = Point.Position + GetActorLocation();
		Transform.SetLocation(Location);

		SpawnedMonster->FinishSpawning(Transform);
		SpawnedMonsters.Add(SpawnedMonster);
	}
}
