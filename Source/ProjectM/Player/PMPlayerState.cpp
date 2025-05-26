#include "PMPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "GameModes/PMExperienceManagerComponent.h"
#include "GameModes/PMGameModeBase.h"
#include "Character/PMPawnData.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "AbilitySystem/PMAbilitySet.h"
#include "AbilitySystem/Attributes/PMHealthSet.h"
#include "AbilitySystem/Attributes/PMCombatSet.h"

#include "Kismet/GameplayStatics.h"
#include "Player/MPlayerSaveGame.h"
#include "GameFramework/PlayerState.h"
#include "Inventory/PMInventoryManagerComponent.h"
#include "Equipment/PMQuickBarComponent.h"
#include "Inventory/PMInventoryItemInstance.h"
#include "GameModes/AsyncAction_ExperienceReady.h"

#include "Equipment/PMEquipmentManagerComponent.h"
#include "Engine/Engine.h"
#include "System/MDataTableManager.h"
#include "PMGameplayTags.h"

#define PLAYER_ID 1
#define PLAYER_NAME TEXT("PlayerName")

APMPlayerState::APMPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UPMAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	CreateDefaultSubobject<UPMHealthSet>(TEXT("HealthSet"));
	CreateDefaultSubobject<UPMCombatSet>(TEXT("CombatSet"));

	NetUpdateFrequency = 100.0f;
	bReplicates = true;
	AbilitySystemComponent->SetIsReplicated(true);
}

void APMPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	{
		const AGameStateBase* GameState = World->GetGameState();
		check(GameState);

		UPMExperienceManagerComponent* ExperienceManagerComp = GameState->FindComponentByClass<UPMExperienceManagerComponent>();
		check(ExperienceManagerComp);

		ExperienceManagerComp->CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
}

void APMPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void APMPlayerState::OnExperienceLoaded(const UPMExperienceDefinition* CurrentExperience)
{
	if (APMGameModeBase* GameMode = GetWorld()->GetAuthGameMode<APMGameModeBase>())
	{
		// PlayerState에 있는 PawnData를 사용해도 되지만 없을수도 있고, 기본적으로 PawnData를 가져오는 곳을 하나로 통일해서 관리를 수월하게 하기 위해 GameMode에서 관리한다.
		const UPMPawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		if (NewPawnData)
		{
			SetPawnData(NewPawnData);
			UAsyncAction_ExperienceReady* ExperienceReady = UAsyncAction_ExperienceReady::WaitForExperienceReady(this);
			if (ExperienceReady)
			{
				ExperienceReady->OnReady.AddDynamic(this, &ThisClass::Server_LoadPlayerData);
				ExperienceReady->Activate();
			}
		}
	}
}

void APMPlayerState::SetPawnData(const UPMPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	check(!PawnData);

	PawnData = InPawnData;

	for (UPMAbilitySet* AbilitySet : PawnData->GetAbilitySets())
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}

void APMPlayerState::Server_SavePlayerData_Implementation()
{
	UpdateCurrentData();
	UGameplayStatics::SaveGameToSlot(PlayerSaveData, PlayerSaveData->SaveSlotName, PlayerSaveData->SaveIndex);
}

void APMPlayerState::Server_LoadPlayerData_Implementation()
{
	if (!PlayerSaveData)
	{
		PlayerSaveData = Cast<UMPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(PLAYER_NAME, PLAYER_ID));
		if (PlayerSaveData)
		{
			ApplyLoadedData();
		}
		else
		{
			APlayerController* Controller = GetPlayerController();
			UPMInventoryManagerComponent* InventoryManager = Controller ? Controller->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
			const TSubclassOf<UPMInventoryItemDefinition>& ItemDef = PawnData->DefaultEquipment;
			if (ItemDef && InventoryManager)
			{
				FMItemRequest Request;

				Request.SetItemRequest(EMItemRequestType::InitItem, ItemDef->GetDefaultObject<UPMInventoryItemDefinition>()->RowId, 1);
				const FMItemHandle& ItemHandle = InventoryManager->RequestItemToInventory(Request);
				InventoryManager->Server_UseItem(ItemHandle);
			}
		}
	}
	else
	{
		ApplyLoadedData();
	}
}

void APMPlayerState::UpdateCurrentData()
{
	if (!PlayerSaveData)
	{
		PlayerSaveData = Cast<UMPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UMPlayerSaveGame::StaticClass()));
	}

	PlayerSaveData->SaveIndex = PLAYER_ID;
	PlayerSaveData->SaveSlotName = PLAYER_NAME;
	APlayerController* Controller = GetPlayerController();
	UPMInventoryManagerComponent* InventoryManager = Controller ? Controller->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
	UPMQuickBarComponent* QuickBarComp = Controller ? Controller->FindComponentByClass<UPMQuickBarComponent>() : nullptr;

	if (InventoryManager && QuickBarComp)
	{
		PlayerSaveData->ItemDefinitions.Empty();

		const FPMInventoryItemList& EquipmentItemList = InventoryManager->GetEquipmentItemList();
		for (const FPMInventoryEntry& Entry : EquipmentItemList.Entries)
		{
			TSubclassOf<UPMInventoryItemDefinition> ItemDef = Entry.GetItemDefinition();
			if (ItemDef)
			{
				PlayerSaveData->ItemDefinitions.Add(ItemDef);
			}
		}

		const FPMInventoryItemList& ConsumableItemList = InventoryManager->GetConsumableItemList();
		for (const FPMInventoryEntry& Entry : ConsumableItemList.Entries)
		{
			TSubclassOf<UPMInventoryItemDefinition> ItemDef = Entry.GetItemDefinition();
			if (ItemDef)
			{
				PlayerSaveData->ItemDefinitions.Add(ItemDef);
			}
		}

		const UPMInventoryItemInstance* EquippedItem = QuickBarComp->GetEquippedItemInstance();
		if (EquippedItem)
		{
			PlayerSaveData->EquippedItem = EquippedItem->ItemDef;
		}
	}
	else
	{
		MCHAE_WARNING("InventoryManager or QuickBarComp is null. Check the controller is created currectly");
	}
}

void APMPlayerState::ApplyLoadedData()
{
	APlayerController* Controller = GetPlayerController();
	if (Controller == nullptr)
	{
		return;
	}

	UPMInventoryManagerComponent* InventoryManager = Controller->FindComponentByClass<UPMInventoryManagerComponent>();
	UPMQuickBarComponent* QuickBarComp = Controller->FindComponentByClass<UPMQuickBarComponent>();

	if (InventoryManager && QuickBarComp)
	{
		for (const TSubclassOf<UPMInventoryItemDefinition>& ItemDef : PlayerSaveData->ItemDefinitions)
		{
			if (ItemDef)
			{
				FMItemRequest Request;
				Request.SetItemRequest(EMItemRequestType::InitItem, ItemDef->GetDefaultObject<UPMInventoryItemDefinition>()->RowId, 1);
				InventoryManager->RequestItemToInventory(Request);
			}
		}

		const TSubclassOf<UPMInventoryItemDefinition>& ItemDef = PawnData->DefaultEquipment;
		if (ItemDef)
		{
			FMItemRequest Request;
			Request.SetItemRequest(EMItemRequestType::InitItem, ItemDef->GetDefaultObject<UPMInventoryItemDefinition>()->RowId, 1);
			const FMItemHandle& ItemHandle = InventoryManager->RequestItemToInventory(Request);
			InventoryManager->Server_UseItem(ItemHandle);
		}
	}
	else
	{
		MCHAE_WARNING("InventoryManager or QuickBarComp is null. Check the controller is created currectly");
	}
}

void APMPlayerState::Debug_SaveGame()
{
	Server_SavePlayerData();
}
