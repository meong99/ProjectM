#include "MBindWidgetByInputComponent.h"
#include "Engine/GameInstance.h"
#include "UI/MViewportClient.h"
#include "PMGameplayTags.h"
#include "UI/MWidgetInstanceList.h"
#include "UI/MWidgetBase.h"
#include "Input/PMInputComponent.h"
#include "Character/PMCharacterBase.h"

UMBindWidgetByInputComponent::UMBindWidgetByInputComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMBindWidgetByInputComponent::BeginPlay()
{
	Super::BeginPlay();

	// 이거 Pawn초기화 다 됐을때로 옮겨야돼
	if (GetNetMode() != ENetMode::NM_DedicatedServer)
	{
		BindWidgetByInput();
	}
}

void UMBindWidgetByInputComponent::BindWidgetByInput()
{
	UMWidgetInstanceList* WidgetInstanceList = GetWidgetInstanceList();
	
	if (WidgetInstanceList == nullptr)
	{
		MCHAE_WARNING("WidgetInstanceList is null!");
		return;
	}

	WidgetInstanceMapWrapper = WidgetInstanceList->GetWidgetInstanceMapWrapper();

	UPMInputComponent* InputComponent = GetInputComponent();
	if (IsValid(InputComponent) == false)
	{
		MCHAE_WARNING("InputComponent is not valid");
		return;
	}

	for (const auto Iter : WidgetInstanceMapWrapper.WidgetInstanceMap)
	{
		if (Iter.Key.IsValid() && IsValid(Iter.Value))
		{
			InputComponent->InputActionMap.Emplace(Iter.Key, FInputActionDelegate::FDelegate::CreateUObject(this, &ThisClass::ToggleWidget));
		}
	}
}

void UMBindWidgetByInputComponent::ToggleWidget(const FGameplayTag& Tag) const
{
	UMWidgetBase* Widget = WidgetInstanceMapWrapper.WidgetInstanceMap.FindRef(Tag);
	if (IsValid(Widget))
	{
		if (Widget->IsInViewport())
		{
			Widget->RemoveFromParent();
		}
		else
		{
			Widget->AddToViewport();
		}
	}
}

UPMInputComponent* UMBindWidgetByInputComponent::GetInputComponent() const
{
	APMCharacterBase* OwnerCharacter = GetPawn<APMCharacterBase>();

	if (IsValid(OwnerCharacter))
	{
		return Cast<UPMInputComponent>(OwnerCharacter->InputComponent);
	}

	return nullptr;
}

UMWidgetInstanceList* UMBindWidgetByInputComponent::GetWidgetInstanceList() const
{
	UGameInstance* GameInstance = GetGameInstance<UGameInstance>();
	UMViewportClient* ViewportClient = GameInstance ? Cast<UMViewportClient>(GameInstance->GetGameViewportClient()) : nullptr;
	if (IsValid(ViewportClient))
	{
		return ViewportClient->CreateWidgetInRegister(FPMGameplayTags::Get().UI_Registry_InputTag);
	}

	return nullptr;
}
