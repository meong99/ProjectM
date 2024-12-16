// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpecHandle.h"
#include "PMAbilitySet.generated.h"

class UPMGameplayAbility;
class UPMAbilitySystemComponent;

// Ability와 Tag를 Wrapping해놓은 구조체
USTRUCT(BlueprintType)
struct FPMAbilitySet_GameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPMGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
};

USTRUCT(BlueprintType)
struct FPMAbilitySet_GrantedHandles 
{
	GENERATED_BODY()

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);

	// 부여된 어빌리티 모두 제거
	void TakeFromAbilitySystem(UPMAbilitySystemComponent* AbilitySystemComp);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
};

/**
 * 
 */
UCLASS()
class PROJECTM_API UPMAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
/*
* Overrided Functions
*/
public:
	UPMAbilitySet();

/*
* Member Functions
*/
public:
	void GiveToAbilitySystem(UPMAbilitySystemComponent* AbilitySystemComp, OUT FPMAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

/*
* Member Variables
*/
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess), Category = "Gameplay Abilities")
	TArray<FPMAbilitySet_GameplayAbility> GrantedGameplayAbilities;
};
