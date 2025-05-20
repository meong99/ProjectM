// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "MNavigationComponent.generated.h"

class UArrowComponent;
class UNavigationPath;

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
	void ActivateNavigation(AActor* InTargetActor, FVector InTargetLocation = FVector::ZeroVector);
	UFUNCTION(BlueprintCallable)
	void DeactivateNavigation();

protected:
	void GeneratePathData();

private:
	void ShowPathPointDebugLine();
	void ShowResearchThresholdDebugLine();
	/*
	* Member Variables
	*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	TSubclassOf<AActor> NavigationGuideActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	float GuideSpeed = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	float NavThreshold = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	float DistanceThreshold = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	float GoalThreshold = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	float ResearchIndexThreshold = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM")
	float RotationSpeed = 5.0f;

	UPROPERTY()
	TObjectPtr<AActor> NavigationActor;

	UPROPERTY()
	TObjectPtr<UNavigationPath> Path;

	int32 CurrentPathIndex = 0;

	TWeakObjectPtr<AActor> TargetWeak;
	FVector TargetLocation;
};
