#pragma once

#include "GameFramework/Character.h"
#include "PMCharacterBase.generated.h"

class UPMPawnExtensionComponent;

UCLASS()
class PROJECTM_API APMCharacterBase : public ACharacter
{
	GENERATED_BODY()

/*
* Engine Functions
*/
public:
	APMCharacterBase();

protected:
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
* Member Functions
*/
public:

/*
* Member Variables
*/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectM | Character")
	TObjectPtr<UPMPawnExtensionComponent> PawnExtComp;
};