#include "PMCharacterBase.h"
#include "Components/PMPawnExtensionComponent.h"
#include "Camera/PMCameraComponent.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "Components/PMHealthComponent.h"
#include "Input/PMInputComponent.h"
#include "UI/MViewportClient.h"

APMCharacterBase::APMCharacterBase()
{
	// 초기화 단계 검사를 틱으로 하지 않고 이벤트 발생 형태로 가기 때문에 틱을 끈다. 하위 클래스에서 필요할 때 켠다.
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	OverrideInputComponentClass = UPMInputComponent::StaticClass();

	PawnExtComp		= CreateDefaultSubobject<UPMPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	PawnExtComp->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtComp->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialzed));

	CameraComponent = CreateDefaultSubobject<UPMCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.f, 0.f, 75.f));

	HealthComponent = CreateDefaultSubobject<UPMHealthComponent>(TEXT("HealthComponent"));
}

void APMCharacterBase::Test_ActivateWidget(const FGameplayTag& RegisterTag, const FGameplayTag& WidgetTag)
{
	UMViewportClient* ViewportClient = UMViewportClient::Get(this);
	if (IsValid(ViewportClient))
	{
		ViewportClient->AddWidgetToViewport(WidgetTag);
	}
}

void APMCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// GameFeature를 통한 초기화를 하고싶은 컴포넌트를 위한 초기화 시작 단계
	PawnExtComp->SetupPlayerInputComponent();

	if (OnSetInputComponentDelegate.IsBound())
	{
		OnSetInputComponentDelegate.Broadcast(InputComponent);
		OnSetInputComponentDelegate.Clear();
	}
}

UAbilitySystemComponent* APMCharacterBase::GetAbilitySystemComponent() const
{
	return PawnExtComp->GetPMAbilitySystemComponent();
}

void APMCharacterBase::OnAbilitySystemInitialized()
{
	UPMAbilitySystemComponent* ASC = Cast<UPMAbilitySystemComponent>(GetAbilitySystemComponent());
	check(ASC);

	HealthComponent->InitializeWithAbilitySystem(ASC);
}

void APMCharacterBase::OnAbilitySystemUninitialzed()
{
	HealthComponent->UninitializeWithAbilitySystem();
}

void APMCharacterBase::CallOrRegister_OnSetInputComponent(FOnSetInputComponent::FDelegate&& Delegate)
{
	if (IsValid(InputComponent))
	{
		Delegate.ExecuteIfBound(InputComponent);
	}
	else
	{
		OnSetInputComponentDelegate.Add(MoveTemp(Delegate));
	}
}
