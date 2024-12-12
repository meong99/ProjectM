// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PMAttributeSet.h"
#include "PMCombatSet.generated.h"

UCLASS()
class PROJECTM_API UPMCombatSet : public UPMAttributeSet
{
	GENERATED_BODY()
public:
	UPMCombatSet();

	ATTRIBUTE_ACCESSORS(UPMCombatSet, BaseHeal);

	UPROPERTY(BlueprintReadOnly, Category = "ProjectM | Combat")
	FGameplayAttributeData BaseHeal;
};
