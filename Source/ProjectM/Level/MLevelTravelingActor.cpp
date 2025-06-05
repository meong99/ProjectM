#include "MLevelTravelingActor.h"
#include "Util/MGameplayStatics.h"
#include "System/MDataTableManager.h"
#include "Engine/Engine.h"
#include "Table/MTable_Level.h"
#include "System/MLevelManager.h"
#include "Engine/LocalPlayer.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MPlayerStart.h"
#include "GameModes/PMGameStateBase.h"
#include "Components/MNavigationComponent.h"
#include "Components/ShapeComponent.h"
#include "TimerManager.h"
#include "Character/MPlayerCharacterBase.h"
#include "Components/CapsuleComponent.h"

AMLevelTravelingActor::AMLevelTravelingActor()
{
	if (!IsRunningDedicatedServer())
	{
		if (GetCollisionComponent())
		{
			GetCollisionComponent()->SetCollisionProfileName(*UEnum::GetDisplayValueAsText(EMCollisionChannel::Interaction).ToString());
		}
	}
}

void AMLevelTravelingActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (HasAuthority())
	{
		GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMLevelTravelingActor::OnBeginOverlap_LevelTravel);
	}

	UMDataTableManager* TableManager = GEngine->GetEngineSubsystem<UMDataTableManager>();
	if (TableManager)
	{
		const UDataTable* LevelTable = TableManager->GetDataTable(EMTableKey::Level);
		if (LevelTable)
		{
			const TArray<FName>& Names = LevelTable->GetRowNames();

			for (const auto& Name : Names)
			{
				const FMTable_Level* TempLevelInfo = LevelTable->FindRow<FMTable_Level>(Name, Name.ToString());
				if (TempLevelInfo && TempLevelInfo->LevelTag == DestLevelTag)
				{
					LevelInfo = TempLevelInfo;
					break;
				}
			}
		}
	}
}

void AMLevelTravelingActor::BeginPlay()
{
	Super::BeginPlay();

	APMGameStateBase* GameState = Cast<APMGameStateBase>(GetWorld()->GetGameState());
	if (GameState && DestLevelTag.IsValid())
	{
		GameState->TagMappedActor.Add(DestLevelTag, this);
	}
}

void AMLevelTravelingActor::OnBeginOverlap_LevelTravel(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMPlayerCharacterBase*	OverlapedPlayer			= Cast<AMPlayerCharacterBase>(OtherActor);
	APlayerController*	OverlapedPlayerController	= OverlapedPlayer ? Cast<APlayerController>(OverlapedPlayer->GetController()) : nullptr;

	if (OverlapedPlayer)
	{
		OverlapedPlayer->Server_AddCharacterStateFlag(EMCharacterStateFlag::BlockMovement);
		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, [this, OverlapedPlayer]()->void
		{
			TeleportToDestination(OverlapedPlayer);
			RequestOngoingNavigation(OverlapedPlayer);
			OverlapedPlayer->SetCurrentLevelTag(DestLevelTag);
			OverlapedPlayer->Server_RemoveCharacterStateFlag(EMCharacterStateFlag::BlockMovement);
		}, 1, false);
	}
}

AActor* AMLevelTravelingActor::FindDestPlayerStart() const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMPlayerStart::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		AMPlayerStart* PlayerStart = Cast<AMPlayerStart>(Actor);
		if (PlayerStart)
		{
			if (PlayerStart->LevelTag == DestLevelTag)
			{
				return PlayerStart;
			}
		}
	}

	return nullptr;
}

AActor* AMLevelTravelingActor::FindDestTravelActor() const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMLevelTravelingActor::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		AMLevelTravelingActor* TravelActor = Cast<AMLevelTravelingActor>(Actor);
		if (TravelActor)
		{
			if (TravelActor->OriginLevelTag == DestLevelTag)
			{
				return TravelActor;
			}
		}
	}

	return nullptr;
}

void AMLevelTravelingActor::TeleportToDestination(AMPlayerCharacterBase* OverlapedPlayer) const
{
	AActor* DestPlayerStart = FindDestPlayerStart();
	if (DestPlayerStart)
	{
		SetPlayerCollisionToTravel(OverlapedPlayer);
		OverlapedPlayer->TeleportTo(DestPlayerStart->GetActorLocation(), DestPlayerStart->GetActorRotation());
		SetPlayerCollisionToOrigin(OverlapedPlayer);
	}
}

void AMLevelTravelingActor::SetPlayerCollisionToTravel(AMPlayerCharacterBase* OverlapedPlayer) const
{
	if (OverlapedPlayer)
	{
		UCapsuleComponent* PlayerCapsule = OverlapedPlayer->GetCapsuleComponent();
		if (PlayerCapsule)
		{
			PlayerCapsule->SetCollisionProfileName(*UEnum::GetDisplayValueAsText(EMCollisionChannel::LavelTavel).ToString());
		}
	}
}

void AMLevelTravelingActor::SetPlayerCollisionToOrigin(AMPlayerCharacterBase* OverlapedPlayer) const
{
	if (OverlapedPlayer)
	{
		UCapsuleComponent* PlayerCapsule = OverlapedPlayer->GetCapsuleComponent();
		if (PlayerCapsule)
		{
			PlayerCapsule->SetCollisionProfileName(*UEnum::GetDisplayValueAsText(EMCollisionChannel::Player).ToString());
		}
	}
}

void AMLevelTravelingActor::RequestOngoingNavigation(AMPlayerCharacterBase* OverlapedPlayer) const
{
	if (OverlapedPlayer)
	{
		if (OverlapedPlayer->IsOnCharacterStateFlags(EMCharacterStateFlag::ControlledFromNavigation))
		{
			UMNavigationComponent* NavComp = OverlapedPlayer->FindComponentByClass<UMNavigationComponent>();
			if (NavComp)
			{
				NavComp->RequestOngoingNavigation();
			}
		}
	}
}
