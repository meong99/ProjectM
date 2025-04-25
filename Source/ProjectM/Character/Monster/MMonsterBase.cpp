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

AMMonsterBase::AMMonsterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (!IsRunningDedicatedServer())
	{
		InteractionComponent = CreateDefaultSubobject<UMInteractionComponent>(TEXT("InteractionComponent"));
		InteractionComponent->SetupAttachment(GetRootComponent());
	}

	HealthComponent = CreateDefaultSubobject<UPMHealthComponent>(TEXT("HealthComponent"));

	AbilitySystemComponent = CreateDefaultSubobject<UPMAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AbilitySystemComponent->bWantsInitializeComponent = true;
	HealthSet = CreateDefaultSubobject<UPMHealthSet>(TEXT("HealthSet"));

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bRequestedMoveUseAcceleration = true;

	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
}

void AMMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetCharacterState(EMCharacterState::Spawned);
	HealthSet->Delegate_OnDamaged.AddDynamic(this, &AMMonsterBase::Callback_OnDamaged);
}

void AMMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	HealthComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
}

void AMMonsterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HealthSet->GetHealth() <= 0.0f && CharacterState & EMCharacterState::Alive)
	{
		SetCharacterState(EMCharacterState::ReadyToDead);
		OnDead();
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

UAbilitySystemComponent* AMMonsterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMMonsterBase::InitMonster(UMMonsterDefinition* InMonsterDefinition, AMMonsterSpawner* InSpawner)
{
	MonsterDefinition = InMonsterDefinition;
	InitCharacterName();
	AbilitySystemComponent->ApplyEffectToSelfWithSetByCaller(nullptr, nullptr, {});





	SetCharacterState(EMCharacterState::Alive);
}

UPMAbilitySystemComponent* AMMonsterBase::GetMAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMMonsterBase::Callback_OnDamaged(AActor* Attacker)
{
	LastAttacker = Attacker;
}

void AMMonsterBase::OnDead()
{
	SetCharacterState(EMCharacterState::Dead);

	APlayerState* AttackerPlayerState = Cast<APlayerState>(LastAttacker);
	if (AttackerPlayerState && MonsterDefinition)
	{
		APlayerController* AttackerController = AttackerPlayerState->GetPlayerController();
		UMWalletComponent* WalletComp = AttackerController ? AttackerController->FindComponentByClass<UMWalletComponent>() : nullptr;
		UPMInventoryManagerComponent* InvenComp = AttackerController ? AttackerController->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
		if (WalletComp)
		{
			WalletComp->AddGold(MonsterDefinition->GetMonsterReward());
		}
		if (InvenComp)
		{
			for (const FMDropInfo& DropInfo : MonsterDefinition->GetItemDropTable())
			{
				#pragma TODO("확률적용해야함")
				InvenComp->AddItemDefinition(DropInfo.ItemId);
			}
		}
	}

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
