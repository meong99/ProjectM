#pragma once

#include "ModularPlayerController.h"
#include "PMPlayerControllerBase.generated.h"

class APMPlayerState;
class UPMAbilitySystemComponent;

UCLASS()
class PROJECTM_API APMPlayerControllerBase : public AModularPlayerController
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	APMPlayerControllerBase();

	void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

/*
* Member Functions
*/
public:
	APMPlayerState* GetPlayerState() const;
	UPMAbilitySystemComponent* GetAbilitySystemComponent() const;
/*
* Member Variables
*/
public:
};