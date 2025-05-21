// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "MLevelTravelingActor.generated.h"

class UBoxComponent;
class UPrimitiveComponent;
class UMLevelManager;
struct FMTable_Level;

/**
 *
 */
UCLASS()
class PROJECTM_API AMLevelTravelingActor : public AActor
{
	GENERATED_BODY()
/*
* Overrided Function
*/
public:
	AMLevelTravelingActor();
	virtual void PostInitializeComponents() override;

/*
* Member Functions
*/
public:
	UFUNCTION()
	void OnBeginOverlap_LevelTravel(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

/*
* Member Variables
*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ProjectM")
	FGameplayTag DestLevelTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ProjectM")
	FGameplayTag OriginLevelTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ProjectM")
	TObjectPtr<UBoxComponent> BoxComponent;

	const FMTable_Level* LevelInfo;
};
