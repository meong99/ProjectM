#include "MMonsterBase.h"
#include "Interaction/MInteractionComponent.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/PMHealthSet.h"
#include "Character/Components/PMHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "Inventory/PMInventoryManagerComponent.h"
#include "Character/Components/MWalletComponent.h"
#include "AIController.h"
#include "MMonsterSpawner.h"
#include "Definitions/MMonsterDefinition.h"
#include "PMGameplayTags.h"
#include "Net/UnrealNetwork.h"
#include "Components/MMonsterTradeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Util/MGameplayStatics.h"
#include "System/MDataTableManager.h"
#include "Table/MTable_MonsterTable.h"

AMMonsterBase::AMMonsterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bRequestedMoveUseAcceleration = true;

	GetCapsuleComponent()->SetCollisionProfileName(*UEnum::GetDisplayValueAsText(EMCollisionChannel::Monster).ToString());

	if (!IsRunningDedicatedServer())
	{
		InteractionComponent = CreateDefaultSubobject<UMInteractionComponent>(TEXT("InteractionComponent"));
		InteractionComponent->SetupAttachment(GetRootComponent());
	}

	AbilitySystemComponent = CreateDefaultSubobject<UPMAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AbilitySystemComponent->bWantsInitializeComponent = true;
	HealthSet = CreateDefaultSubobject<UPMHealthSet>(TEXT("HealthSet"));

	MonsterTradeComponent = CreateDefaultSubobject<UMMonsterTradeComponent>(TEXT("MonsterTradeComponent"));
}

void AMMonsterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMMonsterBase, MonsterDefinition);
}

void AMMonsterBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UMDataTableManager* TableManager = GEngine->GetEngineSubsystem<UMDataTableManager>();

	if (TableManager)
	{
		const UDataTable* MonsterTable = TableManager->GetDataTable(MonsterRowId);
		if (MonsterTable)
		{
			int32 ElementIndex = UMDataTableManager::ChangeRowIdToElementId(MonsterRowId) - 1;
			const TArray<FName>& Names = MonsterTable->GetRowNames();
			if (Names.IsValidIndex(ElementIndex))
			{
				FMTable_MonsterTable* RowData = MonsterTable->FindRow<FMTable_MonsterTable>(Names[ElementIndex], Names[ElementIndex].ToString());
				if (RowData && RowData->Definition)
				{
					MonsterDefinition = DuplicateObject(RowData->Definition->GetDefaultObject<UMMonsterDefinition>(), this);
				}
			}
		}
	}

	if (GetNetMode() != ENetMode::NM_DedicatedServer && IsValid(MonsterDefinition))
	{
		InteractionComponent->SetNewInteractions(MonsterDefinition->Action_OnBeginOverlap, MonsterDefinition->Action_OnInteract);
	}
}

void AMMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		SetCharacterState(EMCharacterState::Spawned);
		HealthSet->Delegate_OnDamaged.AddDynamic(this, &AMMonsterBase::Callback_OnDamaged);

		if (MonsterDefinition)
		{
			TMap<FGameplayTag, float> SetMap;
			SetMap.Add(FPMGameplayTags::Get().Ability_Effect_SetByCaller_Health, MonsterDefinition->GetMonsterHp());
			SetMap.Add(FPMGameplayTags::Get().Ability_Effect_SetByCaller_MaxHealth, MonsterDefinition->GetMonsterHp());
			AbilitySystemComponent->ApplyEffectToSelfWithSetByCaller(MonsterDefinition->GetMonsterInfo().DefaultApplyEffect, nullptr, SetMap);
		}
	}
}

void AMMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SpawnDefaultController();
		SetCharacterState(EMCharacterState::Alive);
	}
}

void AMMonsterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasAuthority())
	{
		if (HealthSet->GetHealth() <= 0.0f && CharacterState & EMCharacterState::Alive)
		{
			SetCharacterState(EMCharacterState::ReadyToDead);
			OnDead();
		}
	}
}

void AMMonsterBase::InitCharacterName()
{
	if (HasAuthority())
	{
		if (MonsterDefinition)
		{
			CharacterName = MonsterDefinition->GetMonsterName();
		}
	}
	Super::InitCharacterName();
}

UAbilitySystemComponent* AMMonsterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMMonsterBase::SetSpawner(AMMonsterSpawner* InSpawner)
{
	Spawner = InSpawner;
}

UPMAbilitySystemComponent* AMMonsterBase::GetMAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMMonsterBase::Callback_OnDamaged(AActor* Attacker)
{
	LastAttacker = Attacker;
}

void AMMonsterBase::GiveRewardToPlayer()
{
	APlayerState* AttackerPlayerState = Cast<APlayerState>(LastAttacker);
	if (AttackerPlayerState && MonsterDefinition)
	{
		APlayerController* AttackerController = AttackerPlayerState->GetPlayerController();
		UMTradeComponentBase* PlayerTradeComponent = AttackerController ? AttackerController->FindComponentByClass<UMTradeComponentBase>() : nullptr;
		if (PlayerTradeComponent)
		{
			FMTradeRequest Request;

			Request.TradeId = Request.GetTradeId();
			Request.GrantGold = MonsterDefinition->GetMonsterReward();
			for (const FMDropInfo& DropInfo : MonsterDefinition->GetItemDropTable())
			{
	#pragma TODO("확률적용해야함")
				Request.GrantItems.ItemRowIds.Add(DropInfo.ItemId);
			}
			Request.RequestType = EMRequestType::Give;
			PlayerTradeComponent->Server_OnRequestSimpleDataGrant(this, Request);
		}
	}
}

void AMMonsterBase::OnDead()
{
	SetCharacterState(EMCharacterState::Dead);
	GiveRewardToPlayer();

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->UnPossess();
	}

	SetCharacterState(EMCharacterState::ReadyToDestroy);

	if (Spawner.IsValid())
	{
		Spawner->OnDeadMonster(this);
	}
	else
	{
		Destroy();
	}
}
