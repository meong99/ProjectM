#include "MAIControllerBase.h"
#include "MMonsterBase.h"
#include "Definitions/MMonsterDefinition.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Types/MBlackboardTypes.h"
#include "Character/MCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Types/MTeamTypes.h"
#include "BehaviorTree/BehaviorTree.h"

AMAIControllerBase::AMAIControllerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius * 1.2;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->SetStartsEnabled(true);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	DamageConfig->SetMaxAge(5.0f);
	DamageConfig->SetStartsEnabled(true);

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComponent->ConfigureSense(*DamageConfig);
	PerceptionComponent->SetDominantSense(DamageConfig->GetSenseImplementation());

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AMAIControllerBase::OnTargetDetectedDelegated);
	PerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &AMAIControllerBase::OnTargetForgotDelegated);
}

void AMAIControllerBase::OnPossess(APawn* InPawn)
{
	AMCharacterBase* OldCharacter = Cast<AMCharacterBase>(GetPawn());
	if (OldCharacter && Handle.IsValid())
	{
		OldCharacter->Delegate_OnChangeCharacterStateFlags.Remove(Handle);
		Handle.Reset();
	}

	Super::OnPossess(InPawn);

	if (AMMonsterBase* Monster = Cast<AMMonsterBase>(InPawn))
	{
		Handle = Monster->Delegate_OnChangeCharacterStateFlags.AddUObject(this, &AMAIControllerBase::OnChange_CharacterStateFlag);
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

void AMAIControllerBase::OnChange_CharacterStateFlag(const int64& OldFlag, const int64& NewFlag)
{
	bool bCanMove = !(NewFlag & EMCharacterStateFlag::BlockMovement);

	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	if (BlackboardComp && UseBlackboard(BlackboardComp->GetBlackboardAsset(), BlackboardComp))
	{
		BlackboardComp->SetValueAsBool(BBKey::CAN_MOVE, bCanMove);
	}
}

void AMAIControllerBase::OnTargetDetectedDelegated(AActor* Actor, struct FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
		if (BlackboardComp && UseBlackboard(BlackboardComp->GetBlackboardAsset(), BlackboardComp))
		{
			BlackboardComp->SetValueAsObject(BBKey::TARGET_ACTOR, Actor);
			BlackboardComp->SetValueAsBool(BBKey::FOUND_PLAYER, true);
		}
	}
}

void AMAIControllerBase::OnTargetForgotDelegated(AActor* Actor)
{
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	if (BlackboardComp && UseBlackboard(BlackboardComp->GetBlackboardAsset(), BlackboardComp))
	{
		BlackboardComp->SetValueAsObject(BBKey::TARGET_ACTOR, nullptr);
		BlackboardComp->SetValueAsBool(BBKey::FOUND_PLAYER, false);
	}
}

ETeamAttitude::Type AMAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	const IGenericTeamAgentInterface* MyTeamAgent = Cast<const IGenericTeamAgentInterface>(GetPawn());
	
	if (OtherTeamAgent && MyTeamAgent)
	{
		FGenericTeamId OtherTeamID = OtherTeamAgent->GetGenericTeamId();
		FGenericTeamId MyTeamID = MyTeamAgent->GetGenericTeamId();
		if (OtherTeamID == MyTeamID)
		{
			return ETeamAttitude::Friendly;
		}
		else if (OtherTeamID == FGenericTeamId::NoTeam)
		{
			return ETeamAttitude::Neutral;
		}
		else
		{
			return ETeamAttitude::Hostile;
		}
	}

	return ETeamAttitude::Neutral;
}
