#include "MEquipmentItemInstance.h"
#include "PMGameplayTags.h"
#include "Player/PMPlayerControllerBase.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

UMEquipmentItemInstance::UMEquipmentItemInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMEquipmentItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, SpawnedActors);
}

int32 UMEquipmentItemInstance::UseItem()
{
	if (CanUseItem() == false)
	{
		return GetStatTagStackCount(FPMGameplayTags::Get().Item_Quentity);
	}

	const UMEquipmentItemDefinition* ItemDefCDO = GetDefault<UMEquipmentItemDefinition>(ItemDef);
	UPMAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (ItemDefCDO && ASC)
	{
		for (const FMApplyEffectDefinition& EffectDef : ItemDefCDO->ApplyEffectToSelf)
		{
			if (EffectDef.EffectClass)
			{
				ASC->ApplyGameplayEffectToSelf(EffectDef.EffectClass->GetDefaultObject<UGameplayEffect>(), EffectDef.EffectLevel, FGameplayEffectContextHandle{});
			}
			else
			{
				MCHAE_ERROR("Effect class is not defined! check item definition! Definition name is %s", *ItemDefCDO->GetName());
			}
		}

		MCHAE_TEST("ItemUse");
	}

	return Super::UseItem();
}

bool UMEquipmentItemInstance::CanUseItem() const
{
	bool bCanUseItem = false;

	if (GetStatTagStackCount(FPMGameplayTags::Get().Item_Quentity))
	{
		bCanUseItem = true;
	}
	if (GetAbilitySystemComponent())
	{
		bCanUseItem &= true;
	}

	return bCanUseItem;
}

void UMEquipmentItemInstance::OnEquipped()
{
}

void UMEquipmentItemInstance::OnUnequipped()
{

}

void UMEquipmentItemInstance::SpawnEquipmentActors(const TArray<FPMEquipmentActorToSpawn>& ActorsToSpawn)
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

void UMEquipmentItemInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}

APawn* UMEquipmentItemInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
	APlayerController* Controller = Cast<APlayerController>(GetOuter());
	if (Controller)
	{
		return Controller->GetPawn();
	}

	return nullptr;
}

APawn* UMEquipmentItemInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		APawn* CurrentPawn = GetPawn();
		if (CurrentPawn && CurrentPawn->IsA(ActualPawnType))
		{
			Result = CurrentPawn;
		}
	}

	return Result;
}

UPMAbilitySystemComponent* UMEquipmentItemInstance::GetAbilitySystemComponent() const
{
	APMPlayerControllerBase* Controller = Cast<APMPlayerControllerBase>(GetOuter());
	if (Controller)
	{
		return Controller->GetAbilitySystemComponent();
	}

	return nullptr;
}
