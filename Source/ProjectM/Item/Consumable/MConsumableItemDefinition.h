#pragma once

#include "CoreMinimal.h"
#include "Inventory/PMInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"
#include "MConsumableItemDefinition.generated.h"

class UGameplayEffect;
class UCurveTable;

USTRUCT(Blueprintable)
struct FMApplyEffectDefinition 
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditDefaultsOnly)
	int32 EffectLevel = 1;
};

UCLASS()
class PROJECTM_API UMConsumableItemDefinition : public UPMInventoryItemDefinition
{
	GENERATED_BODY()

public:
	UMConsumableItemDefinition();

	UPROPERTY(EditDefaultsOnly, Category = "ProjectM | Effect")
	TArray<FMApplyEffectDefinition> ApplyEffectToSelf;
};