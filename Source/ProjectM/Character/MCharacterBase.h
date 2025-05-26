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
	virtual void OnDead();


	const FName&	GetCharacterName() const;
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void	SetCharacterLifeState(const EMCharacterLiftState InState);
	UFUNCTION(BlueprintCallable)
	bool	IsOnCharacterStateFlags(const int64& InState)	{ return CharacterStateFlag & InState; }
	UFUNCTION(BlueprintCallable)
	int64	GetCharacterStateFlag() const					{ return CharacterStateFlag; }
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void	Server_AddCharacterStateFlag(const int64& InState);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void	Server_RemoveCharacterStateFlag(const int64& InState);
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