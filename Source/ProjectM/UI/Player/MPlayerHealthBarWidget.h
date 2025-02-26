// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MWidgetBase.h"
#include "MPlayerHealthBarWidget.generated.h"

class UImage;
class APawn;
class UPMHealthComponent;
class AActor;

UCLASS()
class PROJECTM_API UMPlayerHealthBarWidget : public UMWidgetBase
{
	GENERATED_BODY()

	/*
	* Overrided Functions
	*/
public:
	UMPlayerHealthBarWidget(const FObjectInitializer& ObjectInitializer);

	virtual void	NativeOnInitialized() override;
	virtual void	NativeConstruct() override;

/*
* Member Functions
*/
protected:
	UFUNCTION()
	void Callback_OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UFUNCTION()
	void Callback_OnHealthChanged(UPMHealthComponent* HealthComp, const float OldValue, const float NewValue, AActor* Instigator);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnHealthChanged(UPMHealthComponent* HealthComp, const float OldValue, const float NewValue, AActor* Instigator);
	/*
* Member Variables
*/
protected:
};
