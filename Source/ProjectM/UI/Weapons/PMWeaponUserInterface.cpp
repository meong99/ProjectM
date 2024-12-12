#include "PMWeaponUserInterface.h"
#include "Equipment/PMEquipmentManagerComponent.h"
#include "Weapons/PMWeaponInstance.h"

UPMWeaponUserInterface::UPMWeaponUserInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UPMWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

#pragma TODO("이거 무기 바뀌었는지 틱으로 확인하고 전환시켜주는건데 그냥 델리게이트로 바꾸자")
	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (UPMEquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<UPMEquipmentManagerComponent>())
		{
			if (UPMWeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<UPMWeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					UPMWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					OnWeaponChanged(OldWeapon, CurrentInstance);
				}
			}
		}
	}
}