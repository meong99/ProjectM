#include "MWeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "PMGameplayTags.h"
#include "Item/Equipment/MWeaponItemDefinition.h"
#include "Character/Monster/MMonsterBase.h"

AMWeaponBase::AMWeaponBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	
	WeaponCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCapsule"));
	WeaponCapsule->SetupAttachment(RootComponent);
}

void AMWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	WeaponCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMWeaponBase::OnBeginOverlap);
}

void AMWeaponBase::ActivateWeapon()
{
	WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMWeaponBase::DeactivateWeapon()
{
	WeaponCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OverlappedActors.Empty();
}

void AMWeaponBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMMonsterBase* Monster = Cast<AMMonsterBase>(OtherActor);
	if (Monster && !OverlappedActors.Contains(Monster) && ItemDef)
	{
		UPMAbilitySystemComponent* OwnerAbilitySystem = GetAbilitySystemComponent();
		UMWeaponItemDefinition* WeaponDefCDO = ItemDef->GetDefaultObject<UMWeaponItemDefinition>();

		if (OwnerAbilitySystem && WeaponDefCDO)
		{
			TMap<FGameplayTag, float> SetbyCallerMap;
			#pragma TODO("공격력 적용해야함")
			SetbyCallerMap.Add(FPMGameplayTags::Get().Ability_Effect_SetByCaller_Health, -10/*이거 공격력으로*/);
			OwnerAbilitySystem->ApplyEffectToTargetWithSetByCaller(WeaponDefCDO->DefaultAttackEffectClass, Monster, this, SetbyCallerMap);
		}

		OverlappedActors.Add(Monster);
	}
}

UPMAbilitySystemComponent* AMWeaponBase::GetAbilitySystemComponent() const
{
	AMCharacterBase* Character = GetOwner<AMCharacterBase>();
	if (Character)
	{
		return Character->GetMAbilitySystemComponent();
	}

	return nullptr;
}

