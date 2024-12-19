// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "UI/MWidgetInstanceList.h"
#include "MBindWidgetByInputComponent.generated.h"

class UMWidgetInstanceList;
class UPMInputComponent;

/*
* 입력에 따라 위젯이 켜지고, 꺼지는 동작을 수행할 수 있도록 하는 컴포넌트
*/
UCLASS()
class PROJECTM_API UMBindWidgetByInputComponent : public UPawnComponent
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMBindWidgetByInputComponent(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;

	/*
* Member Functions
*/
public:

private:
	void BindWidgetByInput();

	void ToggleWidget(const FGameplayTag& Tag) const;

	UPMInputComponent*		GetInputComponent() const;
	UMWidgetInstanceList*	GetWidgetInstanceList() const;
/*
* Member Variables
*/
private:
	UPROPERTY()
	FMWidgetInstanceMapWrapper WidgetInstanceMapWrapper;
};
