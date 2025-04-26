#pragma once

#include "MCharacterBase.h"
#include "AbilitySystemInterface.h"
#include "MPlayerCharacterBase.generated.h"

class UPMPawnExtensionComponent;
class UPMCameraComponent;
class UPMHealthComponent;
class UInputComponent;
class UPMAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSetInputComponent, UInputComponent* PlayerInputComponent);

UCLASS()
class PROJECTM_API AMPlayerCharacterBase : public AMCharacterBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	AMPlayerCharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UFUNCTION(BlueprintCallable)
	virtual UPMAbilitySystemComponent* GetMAbilitySystemComponent() const override;

/*
* Member Functions
*/
public:
	void OnAbilitySystemInitialized();
	void OnAbilitySystemUninitialzed();

	void CallOrRegister_OnSetInputComponent(FOnSetInputComponent::FDelegate&& Delegate);
	
	UFUNCTION(BlueprintCallable)
	void Test_ActivateWidget(const FGameplayTag& RegisterTag, const FGameplayTag& WidgetTag);
/*
* Member Variables
*/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectM | Character")
	TObjectPtr<UPMPawnExtensionComponent> PawnExtComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM | Character")
	TObjectPtr<UPMCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectM | Character")
	TObjectPtr<UPMHealthComponent> HealthComponent;

private:
	FOnSetInputComponent OnSetInputComponentDelegate;
};