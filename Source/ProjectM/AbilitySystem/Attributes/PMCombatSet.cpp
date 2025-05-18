#include "PMCombatSet.h"
#include "Net/UnrealNetwork.h"

UPMCombatSet::UPMCombatSet() : Super() , AttackPower(0.0f), DefensePower(0.f)
{
}

void UPMCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPMCombatSet, AttackPower);
	DOREPLIFETIME(UPMCombatSet, DefensePower);
}
