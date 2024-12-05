#pragma once

#include "ModularCharacter.h"
#include "PMCharacterBase.generated.h"

class UPMPawnExtensionComponent;
class UPMCameraComponent;

UCLASS()
class PROJECTM_API APMCharacterBase : public AModularCharacter
{
	GENERATED_BODY()

/*
* Overrided Function
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM | Character")
	TObjectPtr<UPMCameraComponent> CameraComponent;
};