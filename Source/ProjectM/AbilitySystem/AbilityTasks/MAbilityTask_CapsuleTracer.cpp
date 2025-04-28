#include "MAbilityTask_CapsuleTracer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Util/MGameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

UMAbilityTask_CapsuleTracer* UMAbilityTask_CapsuleTracer::CreateCapsuleTracerTask(UGameplayAbility* InOwningAbility, const ACharacter* InOwnerCharacter,
	const FString& InStartSocketName, const FString& InEndSocketName, const TArray<TEnumAsByte<EObjectTypeQuery>>& InObjectTypes, FOnHit&& Callback, const TArray<AActor*>& InActorsToIgnore)
{
	UMAbilityTask_CapsuleTracer* MyObj = NewAbilityTask<UMAbilityTask_CapsuleTracer>(InOwningAbility);
	MyObj->Init(InOwnerCharacter, InStartSocketName, InEndSocketName, InObjectTypes, MoveTemp(Callback), InActorsToIgnore);
	return MyObj;
}

void UMAbilityTask_CapsuleTracer::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	FVector Start = OwnerCharacter->GetMesh()->GetSocketLocation(*StartSocketName);
	FVector End = OwnerCharacter->GetMesh()->GetSocketLocation(*EndSocketName);;

	EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;
	if (UMGameplayStatics::bShowDebug_Console)
	{
		DebugType = EDrawDebugTrace::ForDuration;
	}
	TArray<FHitResult> OutHits;
	UKismetSystemLibrary::CapsuleTraceMultiForObjects(this, Start, End, 22, 60, ObjectTypes, false, ActorsToIgnore, DebugType, OutHits, true);
	if (OutHits.Num())
	{
		TArray<AActor*> HitActors;
		for (const FHitResult& HitResult : OutHits)
		{
			if (HitResult.GetActor())
			{
				HitActors.Add(HitResult.GetActor());
				ActorsToIgnore.Add(HitResult.GetActor());
			}
		}
		Delegate_OnHit.ExecuteIfBound(HitActors);
	}
}

void UMAbilityTask_CapsuleTracer::Activate()
{
	Super::Activate();

	bTickingTask = true;
}

void UMAbilityTask_CapsuleTracer::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
}

void UMAbilityTask_CapsuleTracer::Init(const ACharacter* InOwnerCharacter, const FString& InStartSocketName, const FString& InEndSocketName, const TArray<TEnumAsByte<EObjectTypeQuery>>& InObjectTypes, FOnHit&& Callback, const TArray<AActor*>& InActorsToIgnore)
{
	OwnerCharacter = InOwnerCharacter;
	StartSocketName = InStartSocketName;
	EndSocketName = InEndSocketName;
	ObjectTypes = InObjectTypes;
	Delegate_OnHit = MoveTemp(Callback);
	ActorsToIgnore = InActorsToIgnore;
}
