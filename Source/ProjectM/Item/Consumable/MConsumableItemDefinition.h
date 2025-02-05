//#pragma once
//
//#include "CoreMinimal.h"
//#include "UObject/Object.h"
//#include "GameFramework/Actor.h"
//
//#include "PMEquipmentDefinition.generated.h"
//
//class UPMEquipmentInstance;
//class UPMAbilitySet;
//
//USTRUCT()
//struct FPMEquipmentActorToSpawn
//{
//	GENERATED_BODY()
//
//	// Spawn할 Actor의 클래스. 기능을 가진다기 보다는 실제 월드에 소환되는 클래스
//	UPROPERTY(EditAnywhere, Category = "Equipment")
//	TSubclassOf<AActor> ActorToSpawn;
//
//	UPROPERTY(EditAnywhere, Category = "Equipment")
//	FName AttachSocket;
//
//	UPROPERTY(EditAnywhere, Category = "Equipment")
//	FTransform AttachTransform;
//};
//
//UCLASS(BlueprintType, Blueprintable)
//class PROJECTM_API UPMEquipmentDefinition : public UObject
//{
//	GENERATED_BODY()
//
//public:
//	UPMEquipmentDefinition();
//
//	// 생성된 장비의 Instance. 장비에 관한 기능들을 가진 실제 장비 클래스다. ManagerClass에서 이 Definition을 통해서 EquipmentInstance을 생성한다.
//	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
//	TSubclassOf<UPMEquipmentInstance> InstanceType;
//
//	// Spawn될 장비에 대한 메타데이터
//	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
//	TArray<FPMEquipmentActorToSpawn> ActorsToSpawn;
//
//	// 장비 장착으로 부여될 어빌리티
//	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
//	TArray<TObjectPtr<UPMAbilitySet>> AbilitySetsToGrant;
//};