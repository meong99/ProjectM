#pragma once

#include "ModularCharacter.h"
#include "MCharacterBase.generated.h"

class UMNameWidgetComponent;

enum EMCharacterState
{
	NoFlags			= 0x00000000,
	WaitToSpawn		= 0x00000001,
	Spawned			= 0x00000002,
	Alive			= 0x00000004,
	ReadyToDead		= 0x00000008,
	Dead			= 0x00000010,
	ReadyToDestroy	= 0x00000020,
	Destroied		= 0x00000040,
};

UCLASS()
class PROJECTM_API AMCharacterBase : public AModularCharacter
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
	virtual void InitCharacterName();
	UFUNCTION(BlueprintImplementableEvent)
	void K2_InitCharacterName();

	const FName&	GetCharacterName() const;
	void			SetCharacterState(int32 InState) { CharacterState = InState; }
/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Name")
	UMNameWidgetComponent* NameComponent;

	UPROPERTY(BlueprintReadWrite)
	FName CharacterName = TEXT("None");

	UPROPERTY(BlueprintReadWrite)
	int32 CharacterState = EMCharacterState::WaitToSpawn;
};