// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MItemTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FMSetbyCallerFloat 
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SetByCallerTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Value = 0.0f;
};

USTRUCT(Blueprintable)
struct FMApplyEffectDefinition 
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FMSetbyCallerFloat> EffectValues;
};

UENUM(BlueprintType)
enum class EMEquipmentItemType : uint8
{
	None UMETA(DisplayName = "None"),
	Weapon UMETA(DisplayName = "Weapon"),
};
