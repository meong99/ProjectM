// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
#include "MInteractionComponent.generated.h"

class UMInteractionActivity_Base;
class UInputComponent;
class UPMInputComponent;
class USphereComponent;

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
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	void OnInteract(const FGameplayTag& Tag);
	void Callback_OnSetInputComponent(UInputComponent* InInputComponent);
	void BindDelegate();
	void UnbindDelegate();
	UFUNCTION(BlueprintCallable)
	void ActivateAllOverlapAction();
	UFUNCTION(BlueprintCallable)
	void DeactivateAllOverlapAction();

	UPMInputComponent* GetInputComponent() const;
/*
* Member Variables
*/
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Instanced)
	TArray<TObjectPtr<UMInteractionActivity_Base>> Action_OnBeginOverlap;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Instanced)
	TArray<TObjectPtr<UMInteractionActivity_Base>> Action_OnInteract;
};
