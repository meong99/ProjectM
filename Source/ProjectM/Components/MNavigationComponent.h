// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "GameplayTagContainer.h"
#include "Containers/Queue.h"
#include "MNavigationComponent.generated.h"

class UArrowComponent;
class UNavigationPath;
class AMPlayerCharacterBase;

/**
 *
 */
UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class PROJECTM_API UMNavigationComponent : public UPawnComponent
{
	GENERATED_BODY()

	/*
	* Overrided Function
	*/
public:
	UMNavigationComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	virtual void BeginPlay() override;

	/*
	* Member Functions
	*/
public:
	UFUNCTION(BlueprintCallable)
	void ActivateNavigation(const FGameplayTag& SearchTag, const FGameplayTag& OriginLevelTag, FVector InTargetLocation = FVector::ZeroVector);
	UFUNCTION(BlueprintCallable)
	void DeactivateNavigation();

	void RequestOngoingNavigation();

private:
	void	GeneratePathData();
	void	StopAndCheckDest();

	void	ShowPathPointDebugLine();
	void	ShowResearchThresholdDebugLine();
	FVector	GetFloorLocation(AActor* TargetActor, bool& bHit) const;
	/*
	* Member Variables
	*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	float GuideSpeed = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	float DistanceThreshold = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	float GoalThreshold = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	float ResearchIndexThreshold = 1000.f;

	UPROPERTY()
	TObjectPtr<UNavigationPath> Path;

	UPROPERTY()
	AMPlayerCharacterBase* OwnerCharacter;

	int32 CurrentPathIndex = 0;

	TQueue<FVector> DestQueue;
};
