#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameFramework/Actor.h"
#include "Item/Equipment/MEquipmentItemDefinition.h"

#include "PMEquipmentDefinition.generated.h"

class UPMEquipmentInstance;
class UPMAbilitySet;

UCLASS(BlueprintType, Blueprintable)
class PROJECTM_API UPMEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UPMEquipmentDefinition();

	// 생성된 장비의 Instance. 장비에 관한 기능들을 가진 실제 장비 클래스다. ManagerClass에서 이 Definition을 통해서 EquipmentInstance을 생성한다.
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TSubclassOf<UPMEquipmentInstance> InstanceType;

	// Spawn될 장비에 대한 메타데이터
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TArray<FPMEquipmentActorToSpawn> ActorsToSpawn;

	// 장비 장착으로 부여될 어빌리티
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TArray<TObjectPtr<UPMAbilitySet>> AbilitySetsToGrant;
};