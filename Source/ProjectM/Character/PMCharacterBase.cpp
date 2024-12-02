#include "PMCharacterBase.h"
#include "PMPawnExtensionComponent.h"

APMCharacterBase::APMCharacterBase()
{
	// 초기화 단계 검사를 틱으로 하지 않고 이벤트 발생 형태로 가기 때문에 틱을 끈다. 하위 클래스에서 필요할 때 켠다.
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	PawnExtComp = CreateDefaultSubobject<UPMPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
}

void APMCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComp->SetupPlayerInputComponent();
}

