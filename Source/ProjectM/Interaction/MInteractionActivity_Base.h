// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MInteractionActivity_Base.generated.h"

class UMInteractionComponent;
class APMPlayerControllerBase;

UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew)
class PROJECTM_API UMInteractionActivity_Base : public UObject
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UMInteractionActivity_Base(const FObjectInitializer& ObjectInitializer);

/*
* Member Functions
*/
public:
	virtual void InitAction(UMInteractionComponent* InOwner);
	UFUNCTION(BlueprintCallable)
	virtual void ActivateAction();
	UFUNCTION(BlueprintCallable)
	virtual void DeactivateAction();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_InitAction(UMInteractionComponent* InOwner, APMPlayerControllerBase* Controller);
	UFUNCTION(BlueprintImplementableEvent)
	void K2_ActivateAction();
	UFUNCTION(BlueprintImplementableEvent)
	void K2_DeactivateAction();

/*
* Member Variables
*/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ActionName;

	TWeakObjectPtr<UMInteractionComponent> Owner;
};
