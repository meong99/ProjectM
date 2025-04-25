#include "MAIControllerBase.h"
#include "MMonsterBase.h"
#include "Definitions/MMonsterDefinition.h"

AMAIControllerBase::AMAIControllerBase()
{
}

void AMAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AMMonsterBase* Monster = Cast<AMMonsterBase>(InPawn))
	{
		UMMonsterDefinition* Def = Monster->GetMonsterDefinition();
		if (Def && Def->GetBehaviorTree())
		{
			RunBehaviorTree(Def->GetBehaviorTree());
		}
		else
		{
			MCHAE_ERROR("Monster BehaviorTree is not valid!!!");
		}
	}
}

void AMAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMAIControllerBase::OnUnPossess()
{
	Super::OnUnPossess();

	Destroy();
}
