// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MInteractionComponent.generated.h"

class UMInteractionActivity_Base;
class USphereComponent;

UCLASS()
class PROJECTM_API UMInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMInteractionComponent(const FObjectInitializer& ObjectInitializer);
	virtual void InitializeComponent() override;

/*
* Member Functions
*/
public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* InteractionShpere;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<TObjectPtr<UMInteractionActivity_Base>> Action_OnBeginOverlap;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<TObjectPtr<UMInteractionActivity_Base>> Action_OnInteract;
};
