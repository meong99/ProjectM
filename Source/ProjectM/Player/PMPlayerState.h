#pragma once

#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "PMPlayerState.generated.h"

class UPMExperienceDefinition;
class UPMPawnData;
class UPMAbilitySystemComponent;
class UAbilitySystemComponent;

UCLASS()
class PROJECTM_API APMPlayerState : public APlayerState/*, public IAbilitySystemInterface*/
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	APMPlayerState();

	virtual void PostInitializeComponents() override;
	/*
* Member Functions
*/
public:
	void OnExperienceLoaded(const UPMExperienceDefinition* CurrentExperience);

	template<class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
// 	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	UPMAbilitySystemComponent* GetPMAbilitySystemComponent() const { return AbilitySystemComponent; }

	void SetPawnData(const UPMPawnData* InPawnData);
/*
* Member Variables
*/
private:
	//이미 Definition에 존재하지만, 빠르게 사용하기 위해 캐싱.
	UPROPERTY()
	TObjectPtr<const UPMPawnData> PawnData;

	UPROPERTY(VisibleAnywhere, Category = "ProjectM | PlayerState")
	TObjectPtr<UPMAbilitySystemComponent> AbilitySystemComponent;
};