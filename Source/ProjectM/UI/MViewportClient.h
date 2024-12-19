// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WidgetRegister/MWidgetRegister.h"
#include "Engine/GameViewportClient.h"

#include "MViewportClient.generated.h"

class UMWidgetInstanceList;

/**
 * 
 */
UCLASS()
class PROJECTM_API UMViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;

/*
* Member Functions
*/
public:
	static UMViewportClient* Get(const UObject* WorldContext);

	// 월드 기준 없이 전역적으로 사용되는 위젯의 Register를 Load, Caching해둔다.
	void LoadDefaultWidgetRegister();
	// 월드 단위로 사용되는 WidgetRegister를 등록, 해제한다. 이 부분은 GameFeatureAction으로 정의된다.
	void AddWidgetRegister(const FGameplayTag& RegisterTag, UMWidgetRegister* InWidgetRegister);
	void RemoveWidgetRegister(const FGameplayTag& RegisterTag);

	void CreateWidgetInRegister(const FGameplayTag& RegisterTag);

	void AddWidgetToViewport(const FGameplayTag& WidgetTag);
	void RemoveWidgetFromParent(const FGameplayTag& WidgetTag);

	UMWidgetBase* GetWidgetInstance(const FGameplayTag& WidgetTag);

private:
	UMWidgetInstanceList* CreateNewWidgetInstanceList(const FGameplayTag& RegisterTag);
	UMWidgetRegister* GetWidgetRegister(const FGameplayTag& Tag);
/*
* Member Variables
*/

private:
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UMWidgetRegister>> WidgetRegisterMap;

	UPROPERTY()
	TMap<FGameplayTag/*RegisterTag*/, TObjectPtr<UMWidgetInstanceList>> WidgetInstanceListMap;
};
