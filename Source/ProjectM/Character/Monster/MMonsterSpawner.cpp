#include "MMonsterSpawner.h"
#include "Character/Monster/MMonsterTypes.h"
#include "Engine/Engine.h"
#include "Util/MGameplayStatics.h"
#include "System/MDataTableManager.h"

AMMonsterSpawner::AMMonsterSpawner(const FObjectInitializer& ObjectInitializer)
{
}

void AMMonsterSpawner::PostLoad()
{
	Super::PostLoad();

	if (GEngine && !HasAnyFlags(RF_ClassDefaultObject | RF_Transient) && !MonsterDefinition)
	{
		ChangeMonsterDefinition();
	}
}

void AMMonsterSpawner::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
#if WITH_EDITOR
	FName PropertyName = PropertyChangedEvent.Property->GetFName();
	if (GEngine && !HasAnyFlags(RF_ClassDefaultObject | RF_Transient) && \
		PropertyName == GET_MEMBER_NAME_CHECKED(AMMonsterSpawner, MonsterRowId))
	{
		ChangeMonsterDefinition();
	}
#endif
}

void AMMonsterSpawner::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
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
