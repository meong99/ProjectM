#include "PMControllerComponent_CharacterParts.h"
#include "PMPawnComponent_CharacterParts.h"

UPMControllerComponent_CharacterParts::UPMControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UPMControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (AController* OwningController = GetController<AController>())
		{
			OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void UPMControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	Super::EndPlay(EndPlayReason);
}

UPMPawnComponent_CharacterParts* UPMControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		return ControlledPawn->FindComponentByClass<UPMPawnComponent_CharacterParts>();
	}

	return nullptr;
}

void UPMControllerComponent_CharacterParts::AddCharacterPart(const FPMCharacterPart& NewPart)
{
	AddCharacterPartInternal(NewPart);
}

void UPMControllerComponent_CharacterParts::AddCharacterPartInternal(const FPMCharacterPart& NewPart)
{
	FPMControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;

	if (UPMPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}

void UPMControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (UPMPawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FPMControllerCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->RemoveCharacterPart(Entry.Handle);
		}
	}

	CharacterParts.Reset();
}

void UPMControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (UPMPawnComponent_CharacterParts* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<UPMPawnComponent_CharacterParts>() : nullptr)
	{
		for (FPMControllerCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}

	if (UPMPawnComponent_CharacterParts* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<UPMPawnComponent_CharacterParts>() : nullptr)
	{
		for (FPMControllerCharacterPartEntry& Entry : CharacterParts)
		{
			check(!Entry.Handle.IsValid());
			Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
		}
	}
}