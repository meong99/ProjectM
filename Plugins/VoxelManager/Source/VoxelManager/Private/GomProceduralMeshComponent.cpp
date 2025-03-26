#include "GomProceduralMeshComponent.h"

UGomProceduralMeshComponent::UGomProceduralMeshComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
}

void UGomProceduralMeshComponent::InitializeComponent()
{
	if (GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		SetVisibility(false);
		SetHiddenInGame(true);
		SetCastShadow(false);
		bAffectDistanceFieldLighting = false;
	}
}

void UGomProceduralMeshComponent::Activate(bool bReset)
{
	Super::Activate(bReset);
	if (GetNetMode() != ENetMode::NM_DedicatedServer)
	{
		SetVisibility(true);
		SetHiddenInGame(false);
		SetCastShadow(true);
		bAffectDistanceFieldLighting = true;
	}
}

void UGomProceduralMeshComponent::Deactivate()
{
	Super::Deactivate();

	SetVisibility(false);
	SetHiddenInGame(true);
	SetCastShadow(false);
	bAffectDistanceFieldLighting = false;
}
