#pragma once

#include "GameFramework/Actor.h"
#include "AIController.h"
#include "MAIControllerBase.generated.h"

class UMMonsterDefinition;
class UAISenseConfig_Sight;
class UBehaviorTree;

UCLASS()
class PROJECTM_API AMAIControllerBase : public AAIController
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	AMAIControllerBase();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void OnUnPossess() override;
/*
* Member Functions
*/
public:
	void	OnChange_CharacterStateFlag(const int64& OldFlag, const int64& NewFlag);

	UFUNCTION()
    void    OnTargetDetectedDelegated(AActor* Actor, struct FAIStimulus Stimulus);
	UFUNCTION()
    void    OnTargetForgotDelegated(AActor* Actor);

protected:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

/*
* Member Variables
*/
protected:
	UPROPERTY()
    UAISenseConfig_Sight* SightConfig;

	FDelegateHandle Handle;
};