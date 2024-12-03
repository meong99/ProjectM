// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PMCameraMode.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"

#include "PMCameraComponent.generated.h"

class UPMCameraModeStack;

DECLARE_DELEGATE_RetVal(TSubclassOf<UPMCameraMode>, FPMCameraModeDelegate);

UCLASS( Transient, Within=PMPlayerCameraManager)
class UPMCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

/*
* Engine Functions
*/
public:
	UPMCameraComponent();

	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) final;

/*
* Member Functions
*/
public:
	static UPMCameraComponent* FindCameraComponent(const AActor* Actor) { return Actor ? Actor->FindComponentByClass<UPMCameraComponent>() : nullptr; }
	void UpdateCameraMode();

	UPMCameraModeStack* GetCameraModeStack() const { return CameraModeStack; }

/*
* Member Variables
*/
public:
	FPMCameraModeDelegate DetermineCameraModeDelegate;

private:
	UPROPERTY()
	TObjectPtr<UPMCameraModeStack> CameraModeStack = nullptr;
};
