#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "Types/MCharacterTypes.h"
#include "MCharacterBase.generated.h"

class UMNameWidgetComponent;
class UPMAbilitySystemComponent;

UCLASS()
class PROJECTM_API AMCharacterBase : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	AMCharacterBase(const FObjectInitializer& ObjectInitializer);
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

/*
* Member Functions
*/
public:
	virtual UAbilitySystemComponent*	GetAbilitySystemComponent() const override { return nullptr; }
	virtual UPMAbilitySystemComponent*	GetMAbilitySystemComponent() const { return nullptr; }
	virtual void InitCharacterName();
	UFUNCTION(BlueprintImplementableEvent)
	void K2_InitCharacterName();

	const FName&	GetCharacterName() const;
	
	void	SetCharacterLifeState(const EMCharacterLiftState InState);
	bool	IsOnCharacterStateFlags(const int64& InState)	{ return CharacterStateFlag & InState; }
	int64	GetCharacterStateFlag() const					{ return CharacterStateFlag; }
	void	AddCharacterStateFlag(const int64& InState)		{ CharacterStateFlag |= InState; }
	void	RemoveCharacterStateFlag(const int64& InState)	{ CharacterStateFlag &= ~InState; }
/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Name")
	UMNameWidgetComponent* NameComponent;

	UPROPERTY(BlueprintReadWrite)
	FName CharacterName = TEXT("None");

	UPROPERTY(Replicated, BlueprintReadWrite)
	EMCharacterLiftState CharacterLifeState = EMCharacterLiftState::WaitToSpawn;

	UPROPERTY(Replicated, BlueprintReadWrite)
	int64 CharacterStateFlag = EMCharacterStateFlag::None;
};