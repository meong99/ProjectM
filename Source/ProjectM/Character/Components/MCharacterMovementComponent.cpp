#include "Character/Components/MCharacterMovementComponent.h"

void UMCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
}

void UMCharacterMovementComponent::AddInputVector(FVector WorldVector, bool bForce /*= false*/)
{
	static double a = 0;
	a += WorldVector.SizeSquared();
	//if (a > 1.0)
	{
		if ((int)a % 10 >= 9)
			MCHAE_LOG("VectorSize = %lf", a);
		if (a > 100.0)
		{
			a = 0;
		}
	}
	Super::AddInputVector(WorldVector, bForce);
}
