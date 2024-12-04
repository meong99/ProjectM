#pragma once

#include "GameFeatureAction.h"
#include "GameFeatureAction_WorldActionBase.generated.h"

struct FGameFeatureStateChangeContext;

UCLASS()
class PROJECTM_API UGameFeatureAction_WorldActionBase : public UGameFeatureAction
{
	GENERATED_BODY()

/*
* Engine Functions
*/
public:
	UGameFeatureAction_WorldActionBase();

	// GameFeature가 모두 활성화되고나서 WorldContext를 넘겨줬었는데 그 Context를 기반으로 적용할 월드를 찾는다.
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;

/*
* Member Functions
*/
public:
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) PURE_VIRTUAL(UGameFeatureAction_WorldActionBase::AddToWorld, );

/*
* Member Variables
*/
public:
};