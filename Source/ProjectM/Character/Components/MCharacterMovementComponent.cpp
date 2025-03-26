#include "Character/Components/MCharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GomVoxelWorld.h"

void UMCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGomVoxelWorld::StaticClass(), VoxelWorlds);
	if (VoxelWorlds.IsEmpty())
	{
		MCHAE_WARNING("VoxelWorld is not placed in world!");
	}

	for (AActor* Actor : VoxelWorlds)
	{
		AGomVoxelWorld* VoxelWorld = Cast<AGomVoxelWorld>(Actor);
		if (VoxelWorld)
		{
			VoxelWorld->UpdateVoxelFromLocation(GetActorLocation());
		}
	}
}

void UMCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
}

void UMCharacterMovementComponent::AddInputVector(FVector WorldVector, bool bForce /*= false*/)
{
	static double MovedDist = 0;
	MovedDist += WorldVector.SizeSquared();
	if (MovedDist > 100.0)
	{
		MovedDist = 0;
		for (AActor* Actor : VoxelWorlds)
		{
			AGomVoxelWorld* VoxelWorld = Cast<AGomVoxelWorld>(Actor);
			if (VoxelWorld)
			{
				VoxelWorld->UpdateVoxelFromLocation(GetActorLocation());
			}
		}
	}
	Super::AddInputVector(WorldVector, bForce);
}
