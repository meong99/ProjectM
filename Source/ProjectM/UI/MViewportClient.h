// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonGameViewportClient.h"
#include "GameplayTagContainer.h"
#include "MViewportClient.generated.h"

class UMWidgetRegister;

/**
 * 
 */
UCLASS()
class PROJECTM_API UMViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;

/*
* Member Functions
*/
public:
	static UMViewportClient* Get(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable)
	void ActivateWidget(const FGameplayTag& RegisterTag, const FGameplayTag& WidgetTag);

	UUserWidget* GetWidgetInstance(const FGameplayTag& RegisterTag, const FGameplayTag& WidgetTag);
/*
* Member Variables
*/

private:
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UMWidgetRegister>> WidgetRegisterMap;
};
