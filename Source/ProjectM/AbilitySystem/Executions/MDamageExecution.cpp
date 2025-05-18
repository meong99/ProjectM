#include "MDamageExecution.h"
#include "AbilitySystem/Attributes/PMCombatSet.h"
#include "AbilitySystem/Attributes/PMHealthSet.h"

struct FCombatStatics
{
	FGameplayEffectAttributeCaptureDefinition DefensePower;
	FGameplayEffectAttributeCaptureDefinition AttackPower;
	FGameplayEffectAttributeCaptureDefinition Health;

	FCombatStatics()
	{
		Health = FGameplayEffectAttributeCaptureDefinition(UPMHealthSet::GetHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, true);
		DefensePower = FGameplayEffectAttributeCaptureDefinition(UPMCombatSet::GetDefensePowerAttribute(), EGameplayEffectAttributeCaptureSource::Target, true);
		AttackPower = FGameplayEffectAttributeCaptureDefinition(UPMCombatSet::GetAttackPowerAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FCombatStatics& CombatStatics()
{
	static FCombatStatics Statics;
	return Statics;
}

UMDamageExecution::UMDamageExecution()
{
	RelevantAttributesToCapture.Add(CombatStatics().Health);
	RelevantAttributesToCapture.Add(CombatStatics().DefensePower);
	RelevantAttributesToCapture.Add(CombatStatics().AttackPower);
}

void UMDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	float DefensePower = 0.0f;
	float AttackPower = 0.0f;
	float Health = 0.0f;
	{
		FAggregatorEvaluateParameters EvaluateParameters;

		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CombatStatics().DefensePower, EvaluateParameters, DefensePower);
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CombatStatics().AttackPower, EvaluateParameters, AttackPower);
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CombatStatics().Health, EvaluateParameters, Health);
	}

	const float Damage = AttackPower * (AttackPower / (AttackPower + DefensePower));
	if (Damage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UPMHealthSet::GetHealthAttribute(), EGameplayModOp::Additive, -Damage));
	}
}