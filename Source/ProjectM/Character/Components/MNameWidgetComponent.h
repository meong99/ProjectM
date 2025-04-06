// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "MNameWidgetComponent.generated.h"

class AMNpcBase;
class UMNpcDefinition;
class APawn;
class ACharacter;
class UMNameWidget;

/**
 * 
 */
UCLASS()
class PROJECTM_API UMNameWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMNameWidgetComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitWidget() override;
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

	/*
* Member Functions
*/
public:

protected:
	void InitNameWidget();
	void AdjustNameWidgetCompLocation();

/*
* Member Variables
*/
protected:
	TWeakObjectPtr<APawn> WeakPlayerPawn;
	TWeakObjectPtr<ACharacter> WeakOwnerCharacter;
	UMNameWidget* NameWidget;

	UPROPERTY(EditDefaultsOnly)
	float MinDist = 0.f;
	UPROPERTY(EditDefaultsOnly)
	float MaxDist = 1000.f;
};
