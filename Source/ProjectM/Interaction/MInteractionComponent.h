// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
#include "MInteractionComponent.generated.h"

class UInputComponent;
class UPMInputComponent;
class USphereComponent;
class UMInteractiveAction_OnInteractionBase;
class UMInteractiveAction_OverlapActionBase;

UCLASS(meta = (BlueprintSpawnableComponent))
class PROJECTM_API UMInteractionComponent : public USphereComponent
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMInteractionComponent(const FObjectInitializer& ObjectInitializer);
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

/*
* Member Functions
*/
public:
	void SetNewInteractions(const TArray<UMInteractiveAction_OverlapActionBase*>& OnBeginOverlap, const TArray<UMInteractiveAction_OnInteractionBase*>& OnInteract);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable)
	void ActivateAllOverlapAction();
	UFUNCTION(BlueprintCallable)
	void DeactivateAllOverlapAction();

	const TArray<UMInteractiveAction_OverlapActionBase*>& GetOnBeginOverlapActions() const {return Action_OnBeginOverlap; }
	const TArray<UMInteractiveAction_OnInteractionBase*>& GetOnOnInteractActions() const {return Action_OnInteract; }

protected:
	void OnInteract(const FGameplayTag& Tag);
	void Callback_OnSetInputComponent(UInputComponent* InInputComponent);
	void BindDelegate();
	void UnbindDelegate();

	UPMInputComponent* GetInputComponent() const;
/*
* Member Variables
*/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectM")
	TArray<TObjectPtr<UMInteractiveAction_OverlapActionBase>> Action_OnBeginOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectM")
	TArray<TObjectPtr<UMInteractiveAction_OnInteractionBase>> Action_OnInteract;
};
