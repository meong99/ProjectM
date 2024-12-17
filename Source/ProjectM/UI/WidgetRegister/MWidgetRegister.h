#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MWidgetRegister.generated.h"

class UUserWidget;

USTRUCT(BlueprintType)
struct FMappedWidgetInstances
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UUserWidget>> WidgetInstances;
};

USTRUCT(BlueprintType)
struct FMappedWidgetData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, TSoftClassPtr<UUserWidget>> WidgetData;
};

/*
* 인게임에서 사용할 위젯을 등록
*/
UCLASS()
class PROJECTM_API UMWidgetRegister : public UPrimaryDataAsset
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMWidgetRegister();

/*
* Member Functions
*/
	void SetWorld(const UObject* InWorldContext);
	void AsyncLoadAllWidgets();

	TSubclassOf<UUserWidget>	GetWidgetClass(const FGameplayTag& Tag);
	UUserWidget*				GetWidgetInstanceAndLoadIfNotLoaded(const FGameplayTag& Tag);

protected:
	UUserWidget* AsyncLoadWidgetByTag(const FGameplayTag& Tag);
	UUserWidget* AsyncLoadWidget_Impl(const FGameplayTag& Tag, const TSoftClassPtr<UUserWidget>& WidgetSoftClass);
/*
* Member Variables
*/
public:
	// WidgetRegister들을 구분지을 태그. 특정 위젯군을 찾을 때 사용한다.
	UPROPERTY(EditDefaultsOnly, Category = "WidgetRegister")
	FGameplayTag			RegisterTag;

	// Widget Metadata
	UPROPERTY(EditDefaultsOnly, Category = "WidgetRegister")
	FMappedWidgetData		MappedWidgetData;

	// Widget Instances
	UPROPERTY()
	FMappedWidgetInstances	MappedWidgetInstances;

	UPROPERTY()
	const UObject* WorldContext;
};
