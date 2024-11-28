#include "PMCameraComponent.h"

UPMCameraComponent::UPMCameraComponent()
{
}

UPMCameraComponent* UPMCameraComponent::GetComponent(APlayerController* PC)
{
    return nullptr;
}

void UPMCameraComponent::InitializeComponent()
{
}

void UPMCameraComponent::SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams)
{
}

bool UPMCameraComponent::NeedsToUpdateViewTarget() const
{
    return false;
}

void UPMCameraComponent::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
}

void UPMCameraComponent::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos)
{
}
