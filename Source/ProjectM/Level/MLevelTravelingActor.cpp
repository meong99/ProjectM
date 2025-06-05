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
#include "UI/MViewportClient.h"
#include "PMGameplayTags.h"
#include "UI/Level/MLoadingWidget.h"

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
	WeakPlayer = Cast<AMPlayerCharacterBase>(OtherActor);

	if (WeakPlayer.IsValid())
	{
		FWidgetAnimationDynamicEvent FadeOut;
		FadeOut.BindDynamic(this, &AMLevelTravelingActor::OnFinishedFadeOut);
		PlayLoadingFadeOut(MoveTemp(FadeOut));
	}
}

void AMLevelTravelingActor::PlayLoadingFadeOut(FWidgetAnimationDynamicEvent&& Callback) const
{
	UMViewportClient* ViewClient = UMViewportClient::Get(this);
	if (ViewClient)
	{
		UMLoadingWidget* LoadingWidget = Cast<UMLoadingWidget>(ViewClient->AddWidgetToLayer(FPMGameplayTags::Get().UI_Registry_Game_Loading));
		if (LoadingWidget && WeakPlayer.IsValid())
		{
			WeakPlayer->Server_AddCharacterStateFlag(EMCharacterStateFlag::BlockMovement);
			LoadingWidget->PlayFadeOut(MoveTemp(Callback));
		}
	}
}

void AMLevelTravelingActor::PlayLoadingFadeIn(FWidgetAnimationDynamicEvent&& Callback) const
{
	UMViewportClient* ViewClient = UMViewportClient::Get(this);
	if (ViewClient)
	{
		UMLoadingWidget* LoadingWidget = Cast<UMLoadingWidget>(ViewClient->AddWidgetToLayer(FPMGameplayTags::Get().UI_Registry_Game_Loading));
		if (LoadingWidget)
		{
			LoadingWidget->PlayFadeIn(MoveTemp(Callback));
		}
	}
}

void AMLevelTravelingActor::OnFinishedFadeOut()
{
	if (WeakPlayer.IsValid())
	{
		TeleportToDestination();

		FWidgetAnimationDynamicEvent FadeIn;
		FadeIn.BindDynamic(this, &AMLevelTravelingActor::OnFinishedFadeIn);
		PlayLoadingFadeIn(MoveTemp(FadeIn));
	}
}

void AMLevelTravelingActor::OnFinishedFadeIn()
{
	if (WeakPlayer.IsValid())
	{
		RequestOngoingNavigation();
		WeakPlayer->SetCurrentLevelTag(DestLevelTag);
		WeakPlayer->Server_RemoveCharacterStateFlag(EMCharacterStateFlag::BlockMovement);

		UMViewportClient* ViewClient = UMViewportClient::Get(this);
		if (ViewClient)
		{
			UMLoadingWidget* LoadingWidget = Cast<UMLoadingWidget>(ViewClient->RemoveWidgetFromLayer(FPMGameplayTags::Get().UI_Registry_Game_Loading));
			if (LoadingWidget)
			{
				LoadingWidget->UnbindAnimationBind();
			}
		}
	}

	WeakPlayer = nullptr;
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

void AMLevelTravelingActor::TeleportToDestination() const
{
	AActor* DestPlayerStart = FindDestPlayerStart();
	if (DestPlayerStart && WeakPlayer.IsValid())
	{
		SetPlayerCollisionToTravel();
		WeakPlayer->TeleportTo(DestPlayerStart->GetActorLocation(), DestPlayerStart->GetActorRotation());
		SetPlayerCollisionToOrigin();
	}
}

void AMLevelTravelingActor::SetPlayerCollisionToTravel() const
{
	if (WeakPlayer.IsValid())
	{
		UCapsuleComponent* PlayerCapsule = WeakPlayer->GetCapsuleComponent();
		if (PlayerCapsule)
		{
			PlayerCapsule->SetCollisionProfileName(*UEnum::GetDisplayValueAsText(EMCollisionChannel::LavelTavel).ToString());
		}
	}
}

void AMLevelTravelingActor::SetPlayerCollisionToOrigin() const
{
	if (WeakPlayer.IsValid())
	{
		UCapsuleComponent* PlayerCapsule = WeakPlayer->GetCapsuleComponent();
		if (PlayerCapsule)
		{
			PlayerCapsule->SetCollisionProfileName(*UEnum::GetDisplayValueAsText(EMCollisionChannel::Player).ToString());
		}
	}
}

void AMLevelTravelingActor::RequestOngoingNavigation() const
{
	if (WeakPlayer.IsValid())
	{
		if (WeakPlayer->IsOnCharacterStateFlags(EMCharacterStateFlag::ControlledFromNavigation))
		{
			UMNavigationComponent* NavComp = WeakPlayer->FindComponentByClass<UMNavigationComponent>();
			if (NavComp)
			{
				NavComp->RequestOngoingNavigation();
			}
		}
	}
}
