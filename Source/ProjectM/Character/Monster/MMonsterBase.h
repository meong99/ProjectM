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
class AMMonsterSpawner;

UCLASS(Blueprintable, BlueprintType)
class PROJECTM_API AMMonsterBase : public AMCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	AMMonsterBase(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InitCharacterName() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

/*
* Member Functions
*/
public:
	// Beginplay이전에 호출
	UFUNCTION(BlueprintCallable)
	void InitMonster(UMMonsterDefinition* InMonsterDefinition, AMMonsterSpawner* InSpawner);

	UFUNCTION(BlueprintCallable)
	virtual UPMAbilitySystemComponent* GetMAbilitySystemComponent() const override;

	UMMonsterDefinition* GetMonsterDefinition() { return MonsterDefinition; }

protected:
	UFUNCTION()
	void Callback_OnDamaged(AActor* Attacker);
	void OnDead();
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
	UMMonsterDefinition* MonsterDefinition;

	UPROPERTY(BlueprintReadOnly, Category = "Monster")
	TObjectPtr<UPMHealthSet> HealthSet;

	UPROPERTY(BlueprintReadOnly, Category = "Monster")
	AActor* LastAttacker;

	TWeakObjectPtr<AMMonsterSpawner> Spawner;
};