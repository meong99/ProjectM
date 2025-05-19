#include "MLevelTravelingActor.h"
#include "Components/BoxComponent.h"
#include "Util/MGameplayStatics.h"
#include "System/MDataTableManager.h"
#include "Engine/Engine.h"
#include "Table/MTable_Level.h"
#include "System/MLevelManager.h"
#include "Engine/LocalPlayer.h"
#include "Engine/GameInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MPlayerStart.h"

AMLevelTravelingActor::AMLevelTravelingActor()
{
	if (!IsRunningDedicatedServer())
	{
		BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
		RootComponent = BoxComponent;
		BoxComponent->SetCollisionProfileName(*UEnum::GetDisplayValueAsText(EMCollisionChannel::Interaction).ToString());
	}
}

void AMLevelTravelingActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (HasAuthority())
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMLevelTravelingActor::OnBeginOverlap_LevelTravel);
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
				if (TempLevelInfo && TempLevelInfo->LevelTag == LevelTag)
				{
					LevelInfo = TempLevelInfo;
					break;
				}
			}
		}
	}
}

void AMLevelTravelingActor::OnBeginOverlap_LevelTravel(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter*			OverlapedPlayer				= Cast<ACharacter>(OtherActor);
	APlayerController*	OverlapedPlayerController	= OverlapedPlayer			? Cast<APlayerController>(OverlapedPlayer->GetController()) : nullptr;
	//ULocalPlayer*		OverlapedLocalPlayer		= OverlapedPlayerController ? Cast<ULocalPlayer>(OverlapedPlayerController->GetLocalPlayer()) : nullptr;
	//UMLevelManager*		LavelManager				= OverlapedLocalPlayer		? OverlapedLocalPlayer->GetSubsystem<UMLevelManager>() : nullptr;
	//if (OtherActor && LevelInfo && LavelManager)
	//{
	//	LavelManager->TravelLevel(LevelInfo->UFED, LevelInfo->Ip);
	//}
	//else
	//{
	//	ensure(false);
	//	MCHAE_ERROR("Level info is not valid. check this actor's level tag and level table");
	//}

	if (OverlapedPlayer)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMPlayerStart::StaticClass(), FoundActors);
		for (AActor* Actor : FoundActors)
		{
			AMPlayerStart* PlayerStart = Cast<AMPlayerStart>(Actor);
			if (PlayerStart)
			{
				if (PlayerStart->LevelTag == LevelTag)
				{
					OverlapedPlayer->TeleportTo(PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());
				}
			}
		}
	}
}
