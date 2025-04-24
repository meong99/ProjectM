#pragma once

#include "Character/MCharacterBase.h"
#include "GameplayAbilities/Public/AbilitySystemInterface.h"
#include "Character/Monster/MMonsterTypes.h"
#include "MMonsterBase.generated.h"

class UMInteractionComponent;
class UPMAbilitySystemComponent;
class UPMHealthComponent;
class UPMHealthSet;
class UMMonsterDefinition;

UCLASS(Blueprintable)
class PROJECTM_API AMMonsterBase : public AMCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	AMMonsterBase(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void InitCharacterName() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

/*
* Member Functions
*/
public:
	UFUNCTION(BlueprintCallable)
	void InitMonster(const FMMonsterInfo& InMonsterInfo);

	UFUNCTION(BlueprintCallable)
	UPMAbilitySystemComponent* GetMAbilitySystemComponent() const;
/*
* Member Variables
*/
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Monster")
	TObjectPtr<UPMAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Monster")
	TObjectPtr<UMInteractionComponent> InteractionComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Monster")
	TObjectPtr<UPMHealthComponent> HealthComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Monster")
	FMMonsterInfo MonsterInfo;

	UPROPERTY(BlueprintReadOnly, Category = "Monster")
	TObjectPtr<UPMHealthSet> HealthSet;
};