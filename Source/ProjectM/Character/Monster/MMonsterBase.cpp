#include "MMonsterBase.h"
#include "Interaction/MInteractionComponent.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/PMHealthSet.h"
#include "Character/Components/PMHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}

void AMMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	HealthComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
	const UPMHealthSet* Set = AbilitySystemComponent->GetSet<UPMHealthSet>();

	if (Set)
	{
		MCHAE_LOG("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@name = %s", *Set->GetName());
	}
	else
	{
		MCHAE_LOG("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@못찾음");
	}
}

void AMMonsterBase::InitCharacterName()
{
	Super::InitCharacterName();
	// 	if (MonsterDefinition)
	// 	{
	// 		CharacterName = MonsterDefinition->MonsterName;
	// 	}
}

UAbilitySystemComponent* AMMonsterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMMonsterBase::InitMonster(const FMMonsterInfo& InMonsterInfo)
{
	MonsterInfo = InMonsterInfo;
}

UPMAbilitySystemComponent* AMMonsterBase::GetMAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
