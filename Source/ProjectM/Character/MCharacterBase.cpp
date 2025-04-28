#include "MCharacterBase.h"
#include "Components/MNameWidgetComponent.h"
#include "Net/UnrealNetwork.h"

AMCharacterBase::AMCharacterBase(const FObjectInitializer& ObjectInitializer)
{
	NameComponent = CreateDefaultSubobject<UMNameWidgetComponent>(TEXT("NameComp"));
	NameComponent->SetupAttachment(GetRootComponent());
}

void AMCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMCharacterBase, CharacterState);
	DOREPLIFETIME(AMCharacterBase, CharacterName);
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
	K2_InitCharacterName();
	NameComponent->InitNameWidget();
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
