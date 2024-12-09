#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "PMGameplayAbilityTargetData_SingleTarget.generated.h"

USTRUCT()
struct FPMGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()
public:
	FPMGameplayAbilityTargetData_SingleTargetHit()
		: CartridgeID(-1)
	{}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FPMGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}

	// 탄약 ID (카트리지)
	UPROPERTY()
	int32 CartridgeID;
};