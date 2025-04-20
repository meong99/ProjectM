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
	InitCharacterName();
}

void AMCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMCharacterBase::InitCharacterName()
{
	K2_InitCharacterName();
}

const FName& AMCharacterBase::GetCharacterName() const
{
	return CharacterName;
}
