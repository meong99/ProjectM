#pragma once

#include "GameFramework/Actor.h"
#include "AIController.h"
#include "MAIControllerBase.generated.h"

class UMMonsterDefinition;

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
/*
* Member Variables
*/
protected:
};