#include "PMCameraComponent.h"
#include "PMCameraMode.h"

UPMCameraComponent::UPMCameraComponent()
{
}

void UPMCameraComponent::OnRegister()
{
    Super::OnRegister();

    if (!CameraModeStack)
    {
        CameraModeStack = NewObject<UPMCameraModeStack>(this);
    }
}

UE_DISABLE_OPTIMIZATION
void UPMCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
    check(CameraModeStack);

    UpdateCameraMode();

    FPMCameraModeView CameraModeView;
    CameraModeStack->EvaluateStack(DeltaTime, CameraModeView);
}

void UPMCameraComponent::UpdateCameraMode()
{
    check(CameraModeStack);

    if (DetermineCameraModeDelegate.IsBound())
    {
        if (const TSubclassOf<UPMCameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
        {
            CameraModeStack->PushCameraMode(CameraMode);
        }
    }
}
UE_ENABLE_OPTIMIZATION
