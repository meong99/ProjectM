#pragma once

#include "Character/MCharacterBase.h"
#include "Character/Monster/MMonsterTypes.h"
#include "AbilitySystem/PMAbilitySet.h"
#include "MMonsterBase.generated.h"

class UMInteractionComponent;
class UPMAbilitySystemComponent;
class UPMHealthComponent;
class UPMHealthSet;
class UMMonsterDefinition;
class AMMonsterSpawner;
class UMMonsterTradeComponent;

UCLASS(Blueprintable, BlueprintType)
class PROJECTM_API AMMonsterBase : public AMCharacterBase
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	AMMonsterBase(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InitCharacterName() override;

	virtual void OnDead() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
/*
* Member Functions
*/
public:
	// Beginplay이전에 호출
	UFUNCTION(BlueprintCallable)
	void SetSpawner(AMMonsterSpawner* InSpawner);

	UFUNCTION(BlueprintCallable)
	virtual UPMAbilitySystemComponent* GetMAbilitySystemComponent() const override;

	UMMonsterDefinition* GetMonsterDefinition() { return MonsterDefinition; }

protected:
	UFUNCTION()
	void Callback_OnDamaged(AActor* Attacker);
	void GiveRewardToPlayer();
/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 1400001, ClampMax = 1499999), Category = "ProjectM")
	int32 MonsterRowId = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster")
	TObjectPtr<UPMAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster")
	TObjectPtr<UMInteractionComponent> InteractionComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster")
	TObjectPtr<UMMonsterTradeComponent> MonsterTradeComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Monster")
	UMMonsterDefinition* MonsterDefinition;

	UPROPERTY(BlueprintReadOnly, Category = "Monster")
	TObjectPtr<UPMHealthSet> HealthSet;

	UPROPERTY(BlueprintReadOnly, Category = "Monster")
	AActor* LastAttacker;

	UPROPERTY()
	TMap<int32, FPMAbilitySet_GrantedHandles> GrantedHandles;

	TWeakObjectPtr<AMMonsterSpawner> Spawner;
};