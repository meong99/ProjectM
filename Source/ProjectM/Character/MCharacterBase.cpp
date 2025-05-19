#include "MCharacterBase.h"
#include "Components/MNameWidgetComponent.h"
#include "Net/UnrealNetwork.h"

AMCharacterBase::AMCharacterBase(const FObjectInitializer& ObjectInitializer)
{
	NameComponent = CreateDefaultSubobject<UMNameWidgetComponent>(TEXT("NameComp"));
	NameComponent->SetupAttachment(GetRootComponent());

	bReplicates = true;
}

void AMCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMCharacterBase, CharacterState);
}

void AMCharacterBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void AMCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	InitCharacterName();
}

void AMCharacterBase::InitCharacterName()
{
	NameComponent->InitNameWidget();
	K2_InitCharacterName();
}

const FName& AMCharacterBase::GetCharacterName() const
{
	return CharacterName;
}

void AMCharacterBase::SetCharacterState(int32 InState)
{
	if (InState > CharacterState)
	{
		CharacterState = InState;
	}
}
