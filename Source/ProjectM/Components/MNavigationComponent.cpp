#include "MNavigationComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/HitResult.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Util/MGameplayStatics.h"

UMNavigationComponent::UMNavigationComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMNavigationComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ArrowActor && Path && GetOwner())
	{
		if (0 <= CurrentPathIndex && CurrentPathIndex < Path->PathPoints.Num() - 1)
		{
			FVector OwnerLocation = GetOwner()->GetActorLocation();
			FVector NextPointLocation = Path->PathPoints[CurrentPathIndex + 1];
			float NextDist = FVector::Dist(OwnerLocation, NextPointLocation);

			FVector CurrentPointLocation = Path->PathPoints[CurrentPathIndex];
			float CurrentDist = FVector::Dist(OwnerLocation, CurrentPointLocation);


			if (CurrentDist > ResearchIndexThreshold)
			{
				GeneratePathData();
			}
			else if (NextDist > DistanceThreshold)
			{
				FVector OriginLocation2D = { OwnerLocation.X, OwnerLocation.Y, 0.0f };
				FVector NextPointLocation2D = { NextPointLocation.X, NextPointLocation.Y, 0.0f };

				FRotator CurrentRot = ArrowActor->GetActorRotation();
				FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(OriginLocation2D, NextPointLocation2D);
				FRotator NewRot = FMath::RInterpTo(CurrentRot, LookAt, DeltaTime, RotationSpeed);
				ArrowActor->SetActorRotation(NewRot);
			}
			else
			{
				CurrentPathIndex++;
			}

#if WITH_EDITOR
			ShowResearchThresholdDebugLine();
#endif
		}
		else
		{
			DeactivateNavigation();
		}
	}
	else
	{
		// 무조건 있어야하는 뭔가가 없음!
		ensure(false);
		DeactivateNavigation();
	}
}

void UMNavigationComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	if (ArrowActorClass)
	{
		ArrowActor = GetWorld()->SpawnActor<AActor>(ArrowActorClass, GetOwner()->GetActorTransform());
		ArrowActor->SetActorEnableCollision(false);
		ArrowActor->SetActorHiddenInGame(true);
		ArrowActor->AttachToActor(GetOwner(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	else
	{
		//ArrowClass가 없음!! 캐릭터 BP에 들어가서 설정하시오
		ensure(false);
	}
}

void UMNavigationComponent::ActivateNavigation(AActor* InTargetActor, FVector InTargetLocation /*= {}*/)
{
	TargetWeak = InTargetActor;
	TargetLocation = InTargetLocation;

	if (ArrowActor)
	{
		ArrowActor->SetActorHiddenInGame(false);
	}

	GeneratePathData();

	SetComponentTickEnabled(true);
}

void UMNavigationComponent::DeactivateNavigation()
{
	if (ArrowActor)
	{
		ArrowActor->SetActorHiddenInGame(true);
	}

	SetComponentTickEnabled(false);
}

void UMNavigationComponent::GeneratePathData()
{
	CurrentPathIndex = 0;

	FVector Dest;

	if (TargetWeak.IsValid())
	{
		Dest = TargetWeak->GetActorLocation();
	}
	else
	{
		Dest = TargetLocation;
	}

	if (Dest.IsZero())
	{
		MCHAE_WARNING("Target vector is zero!");
		return;
	}

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		MCHAE_WARNING("OwnerActor is not valid");
		return;
	}

	FVector PathStart = OwnerActor->GetActorLocation();

	Path = UNavigationSystemV1::FindPathToLocationSynchronously(this, PathStart, Dest);
	if (Path && Path->PathPoints.Num() >= 2)
	{
		FVector NextPointLocation = Path->PathPoints[CurrentPathIndex + 1];
		NextPointLocation = { NextPointLocation.X, NextPointLocation.Y, 0.0f };
		float Dist = FVector::Dist(PathStart, NextPointLocation);

		ResearchIndexThreshold = Dist;
	}
	else
	{
		ResearchIndexThreshold = 1000.f;
	}

#if WITH_EDITOR
	ShowPathPointDebugLine();
#endif
}

void UMNavigationComponent::ShowPathPointDebugLine()
{
	if (UMGameplayStatics::bShowDebug_Console && Path && Path->PathPoints.Num() > 0)
	{
		TArray<FHitResult> Result;
		for (int32 i = 0; i < Path->PathPoints.Num(); i++)
		{
			UKismetSystemLibrary::LineTraceMulti(this, Path->PathPoints[i], Path->PathPoints[i] + (FVector::UpVector * 100),
				ETraceTypeQuery::TraceTypeQuery1, false, {}, EDrawDebugTrace::ForDuration, Result, true, FLinearColor::Red, FLinearColor::Green, 10);
		}
	}
}

void UMNavigationComponent::ShowResearchThresholdDebugLine()
{
	if (UMGameplayStatics::bShowDebug_Console)
	{
		TArray<FHitResult> Result;
		FVector Origin = Path->PathPoints[CurrentPathIndex] + FVector::UpVector * 100;
		FVector OwnerLoc = GetOwner()->GetActorLocation();
		FVector ThresholdLoc = Origin + (OwnerLoc - Origin).GetSafeNormal() * ResearchIndexThreshold;

		UKismetSystemLibrary::LineTraceMulti(this, Origin, ThresholdLoc,
			ETraceTypeQuery::TraceTypeQuery1, false, {}, EDrawDebugTrace::ForOneFrame, Result, true, FLinearColor::Red, FLinearColor::Green);
	}
}
