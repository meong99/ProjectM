#include "PMEquipmentInstance.h"
#include "GameFramework/Character.h"
#include "PMEquipmentDefinition.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

UPMEquipmentInstance::UPMEquipmentInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

APawn* UPMEquipmentInstance::GetPawn() const
{
    return Cast<APawn>(GetOuter());
}

APawn* UPMEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}

	return Result;
}

void UPMEquipmentInstance::SpawnEquipmentActors(const TArray<FPMEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Character = Cast<ACharacter>(OwningPawn))
		{
			AttachTarget = Character->GetMesh();
		}

		for (const FPMEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, true);

			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);

			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

			SpawnedActors.Add(NewActor);
		}
	}
}

void UPMEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

void UPMEquipmentInstance::OnEquipped()
{
#pragma TODO("Native화 필요!")
	K2_OnEquipped();
}

void UPMEquipmentInstance::OnUnequipped()
{
#pragma TODO("Native화 필요!")
	K2_OnUnequipped();
}
