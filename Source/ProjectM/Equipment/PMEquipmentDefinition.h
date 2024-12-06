#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameFramework/Actor.h"

#include "PMEquipmentDefinition.generated.h"

class UPMEquipmentInstance;

USTRUCT()
struct FPMEquipmentActorToSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Equipment")
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category = "Equipment")
	FTransform AttachTransform;
};

UCLASS(BlueprintType, Blueprintable)
class PROJECTM_API UPMEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UPMEquipmentDefinition();

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TSubclassOf<UPMEquipmentInstance> InstanceType;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TArray<FPMEquipmentActorToSpawn> ActorsToSpawn;
};