#pragma once

#include "CoreMinimal.h"
#include "Equipment/MEquipableActorBase.h"
#include "MWeaponBase.generated.h"

class UStaticMeshComponent;
class UCapsuleComponent;
class UPrimitiveComponent;
class UPMAbilitySystemComponent;

/**
 *
 */
UCLASS()
class PROJECTM_API AMWeaponBase : public AMEquipableActorBase
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	AMWeaponBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void PostInitializeComponents() override;

	/*
* Member Functions
*/
public:
	UFUNCTION(BlueprintCallable)
	virtual void ActivateWeapon();

	UFUNCTION(BlueprintCallable)
	virtual void DeactivateWeapon();

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPMAbilitySystemComponent* GetAbilitySystemComponent() const;

/*
* Member Variables
*/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UCapsuleComponent* WeaponCapsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY()
	TSet<AActor*> OverlappedActors;
};