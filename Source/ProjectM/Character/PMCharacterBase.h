#pragma once

#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "PMCharacterBase.generated.h"

class UPMPawnExtensionComponent;
class UPMCameraComponent;
class UPMHealthComponent;
class UInputComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSetInputComponent, UInputComponent* PlayerInputComponent);

UCLASS()
class PROJECTM_API APMCharacterBase : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	APMCharacterBase();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

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