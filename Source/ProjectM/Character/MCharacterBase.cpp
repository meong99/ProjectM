#include "MCharacterBase.h"
#include "Components/MNameWidgetComponent.h"

AMCharacterBase::AMCharacterBase(const FObjectInitializer& ObjectInitializer)
{
	NameComponent = CreateDefaultSubobject<UMNameWidgetComponent>(TEXT("NameComp"));
	NameComponent->SetupAttachment(GetRootComponent());
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
