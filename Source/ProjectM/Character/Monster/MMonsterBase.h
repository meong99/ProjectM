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

UCLASS()
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
// 	UMMonsterDefinition* GetMonsterDefinition() const { return MonsterDefinition; }
	UFUNCTION(BlueprintCallable)
	void InitMonster(const FMMonsterInfo& InMonsterInfo);

	UFUNCTION(BlueprintCallable)
	UPMAbilitySystemComponent* GetMAbilitySystemComponent() const;
/*
* Member Variables
*/
protected:
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "NPC")
// 	UMMonsterDefinition* MonsterDefinition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster")
	TObjectPtr<UPMAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster")
	TObjectPtr<UMInteractionComponent> InteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster")
	TObjectPtr<UPMHealthComponent> HealthComponent;

	#pragma TODO("이거 삭제")
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Monster")
	UMMonsterDefinition* MonsterDefinition;

	UPROPERTY(BlueprintReadOnly, Category = "Monster")
	FMMonsterInfo MonsterInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	TObjectPtr<UPMHealthSet> HealthSet;
};