#pragma once

#include "ModularCharacter.h"
#include "MCharacterBase.generated.h"

class UMNameWidgetComponent;

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

	const FName& GetCharacterName() const;
/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Name")
	UMNameWidgetComponent* NameComponent;

	UPROPERTY(BlueprintReadWrite)
	FName CharacterName = TEXT("None");
};