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
