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
#include "AbilitySystem/PMAbilitySet.h"
#include "AbilitySystem/Attributes/PMCombatSet.h"
#include "Types/MTeamTypes.h"

AMMonsterBase::AMMonsterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bRequestedMoveUseAcceleration = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

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
	CombatSet = CreateDefaultSubobject<UPMCombatSet>(TEXT("CombatSet"));

	MonsterTradeComponent = CreateDefaultSubobject<UMMonsterTradeComponent>(TEXT("MonsterTradeComponent"));
}

void AMMonsterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
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
					GetCharacterMovement()->MaxWalkSpeed = MonsterDefinition->GetMonsterInfo().MonsterMaxSpeed;
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
		SetCharacterLifeState(EMCharacterLiftState::Spawned);
		HealthSet->Delegate_OnDamaged.AddDynamic(this, &AMMonsterBase::Callback_OnDamaged);

		if (MonsterDefinition)
		{
			TMap<FGameplayTag, float> SetMap;
			SetMap.Add(FPMGameplayTags::Get().Ability_Effect_SetByCaller_Health, MonsterDefinition->GetMonsterHp());
			SetMap.Add(FPMGameplayTags::Get().Ability_Effect_SetByCaller_MaxHealth, MonsterDefinition->GetMonsterHp());
			SetMap.Add(FPMGameplayTags::Get().Ability_Effect_SetByCaller_AttackPower, MonsterDefinition->MonsterCombatInfo.MonsterAttackPower);
			SetMap.Add(FPMGameplayTags::Get().Ability_Effect_SetByCaller_DefensePower, MonsterDefinition->MonsterCombatInfo.MonsterDefensePower);
			AbilitySystemComponent->ApplyEffectToSelfWithSetByCaller(MonsterDefinition->GetMonsterInfo().DefaultApplyEffect, nullptr, SetMap);

			FPMAbilitySet_GrantedHandles TempGrantedHandles;
			for (const UPMAbilitySet* AbilitySet : MonsterDefinition->AbilitySets)
			{
				AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &TempGrantedHandles, this, MonsterDefinition->RowId);
			}

			GrantedHandles.Add(MonsterDefinition->RowId, TempGrantedHandles);
		}
	}
}

void AMMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetCharacterLifeState(EMCharacterLiftState::Alive);
	}

	SetGenericTeamId((int32)EMGenericTeamId::Monster);
}

void AMMonsterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasAuthority())
	{
		if (HealthSet->GetHealth() <= 0.0f && CharacterLifeState == EMCharacterLiftState::Alive)
		{
			SetCharacterLifeState(EMCharacterLiftState::ReadyToDead);
			OnDead();
		}
	}
}

void AMMonsterBase::InitCharacterName()
{
	if (MonsterDefinition)
	{
		CharacterName = MonsterDefinition->GetMonsterName();
	}
	Super::InitCharacterName();
}

void AMMonsterBase::OnDead()
{
	Super::OnDead();

	SetCharacterLifeState(EMCharacterLiftState::Dead);
	GiveRewardToPlayer();

	if (HasAuthority())
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			AIController->UnPossess();
		}

		SetCharacterLifeState(EMCharacterLiftState::ReadyToDestroy);

		if (MonsterDefinition->DeathAnimation.Montage)
		{
			float Duration = MonsterDefinition->DeathAnimation.Montage->GetPlayLength();
			Multicast_PlayMontage(MonsterDefinition->DeathAnimation.Montage);
			UMGameplayStatics::SetTimer(this, [this]()->void
			{
				if (Spawner.IsValid())
				{
					Spawner->OnDeadMonster(this);
				}

				Destroy();
			}, Duration, false);
		}
	}
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
			FMTradeRequest Request = MonsterDefinition->CreateTradeRequest();
			PlayerTradeComponent->Server_OnRequestSimpleDataGrant(this, Request);
		}
	}
}

UAbilitySystemComponent* AMMonsterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
