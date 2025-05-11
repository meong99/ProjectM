#pragma once

#include "CommonPlayerController.h"
#include "GameModes/PMExperienceManagerComponent.h"
#include "Character/MPlayerCharacterBase.h"
#include "PMPlayerControllerBase.generated.h"

class APMPlayerState;
class UPMAbilitySystemComponent;
class UPMInventoryManagerComponent;
class UMPlayerTradeComponent;
class UMPlayerQuestComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPossessed, AMPlayerCharacterBase* /*Character*/);

UCLASS()
class PROJECTM_API APMPlayerControllerBase : public ACommonPlayerController
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	APMPlayerControllerBase();

	virtual void OnPossess(APawn* aPawn) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

/*
* Member Functions
*/
public:
	void CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate&& Delegate);
	void CallOrRegister_OnPossessed(FOnPossessed::FDelegate&& Delegate);

	APMPlayerState* GetPlayerState() const;
	UPMAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void Debug_WidgetControl(const FGameplayTag& WidgetTag, bool bAddWidget, UObject* WidgetInstigator = nullptr);
/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPMInventoryManagerComponent> InventoryManagerComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMPlayerTradeComponent> PlayerTradeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMPlayerQuestComponent> PlayerQuestComponent;

	FOnPossessed Delegate_OnPossessed;
};