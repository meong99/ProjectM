#include "MPlayerCharacterBase.h"
#include "Components/PMPawnExtensionComponent.h"
#include "Camera/PMCameraComponent.h"
#include "AbilitySystem/PMAbilitySystemComponent.h"
#include "Components/PMHealthComponent.h"
#include "Input/PMInputComponent.h"
#include "UI/MViewportClient.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/MCharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Util/MGameplayStatics.h"
#include "Components/MNavigationComponent.h"
#include "Components/PMCharacterInitComponent.h"

AMPlayerCharacterBase::AMPlayerCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// 초기화 단계 검사를 틱으로 하지 않고 이벤트 발생 형태로 가기 때문에 틱을 끈다. 하위 클래스에서 필요할 때 켠다.
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	OverrideInputComponentClass = UPMInputComponent::StaticClass();

	GetCapsuleComponent()->SetCollisionProfileName(*UEnum::GetDisplayValueAsText(EMCollisionChannel::Player).ToString());

	PawnExtComp	= CreateDefaultSubobject<UPMPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	PawnExtComp->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtComp->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialzed));

	CameraComponent = CreateDefaultSubobject<UPMCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetRelativeLocation(FVector(-300.f, 0.f, 75.f));

	HealthComponent = CreateDefaultSubobject<UPMHealthComponent>(TEXT("HealthComponent"));
	
	NetCullDistanceSquared = 900000000.0f;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesAndRefreshBonesWhenPlayingMontages;

	if (!IsRunningDedicatedServer())
	{
		NavigationComponent = CreateDefaultSubobject<UMNavigationComponent>(TEXT("NavigationComponent"));
	}
}

void AMPlayerCharacterBase::Test_ActivateWidget(const FGameplayTag& RegisterTag, const FGameplayTag& WidgetTag)
{
	UMViewportClient* ViewportClient = UMViewportClient::Get(this);
	if (IsValid(ViewportClient))
	{
		ViewportClient->AddWidgetToViewport(WidgetTag);
	}
}

void AMPlayerCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Dedi에서도 Controller가 존재할 때 초기화 단계를 다시 시작해주는 부분이 필요하다.
	// Client는 SetupPlayerInputComponent에서 실행
	if (GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		PawnExtComp->HandleControllerChanged();
	}
}

void AMPlayerCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// GameFeature를 통한 초기화를 하고싶은 컴포넌트를 위한 초기화 시작 단계
	PawnExtComp->SetupPlayerInputComponent();

	UPMCharacterInitComponent* CharacterInitComp = FindComponentByClass<UPMCharacterInitComponent>();
	if (InputComponent != nullptr && CharacterInitComp)
	{
		CharacterInitComp->InitializePlayerInput(InputComponent);
	}

	if (OnSetInputComponentDelegate.IsBound())
	{
		OnSetInputComponentDelegate.Broadcast(InputComponent);
		OnSetInputComponentDelegate.Clear();
	}
}

UAbilitySystemComponent* AMPlayerCharacterBase::GetAbilitySystemComponent() const
{
	return PawnExtComp->GetPMAbilitySystemComponent();
}

UPMAbilitySystemComponent* AMPlayerCharacterBase::GetMAbilitySystemComponent() const
{
	return Cast<UPMAbilitySystemComponent>(GetAbilitySystemComponent());
}

void AMPlayerCharacterBase::OnAbilitySystemInitialized()
{
	UPMAbilitySystemComponent* ASC = Cast<UPMAbilitySystemComponent>(GetAbilitySystemComponent());
	check(ASC);

	HealthComponent->InitializeWithAbilitySystem(ASC);
}

void AMPlayerCharacterBase::OnAbilitySystemUninitialzed()
{
	HealthComponent->UninitializeWithAbilitySystem();
}

void AMPlayerCharacterBase::CallOrRegister_OnSetInputComponent(FOnSetInputComponent::FDelegate&& Delegate)
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
