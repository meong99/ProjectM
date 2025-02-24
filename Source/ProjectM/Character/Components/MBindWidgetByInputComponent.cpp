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

	if (GetNetMode() != ENetMode::NM_DedicatedServer)
	{
		APMCharacterBase* OwnerCharacter = GetPawn<APMCharacterBase>();
		if (IsValid(OwnerCharacter))
		{
			OwnerCharacter->CallOrRegister_OnSetInputComponent(FOnSetInputComponent::FDelegate::CreateUObject(this, &ThisClass::BindWidgetByInput));
		}
	}
}

void UMBindWidgetByInputComponent::BindWidgetByInput(UInputComponent* InInputComponent)
{
	UMWidgetInstanceList* WidgetInstanceList = GetWidgetInstanceList();
	
	if (WidgetInstanceList == nullptr)
	{
		MCHAE_WARNING("WidgetInstanceList is null!");
		return;
	}

	WidgetInstanceMapWrapper = WidgetInstanceList->GetWidgetInstanceMapWrapper();
	check(WidgetInstanceMapWrapper);

	InputComponent = Cast<UPMInputComponent>(InInputComponent);
	if (IsValid(InputComponent) == false)
	{
		MCHAE_WARNING("InputComponent is not valid");
		return;
	}

	const auto& WidgetInstanceMap = WidgetInstanceMapWrapper->WidgetInstanceMap;
	for (const auto Iter : WidgetInstanceMap)
	{
		if (Iter.Key.IsValid() && IsValid(Iter.Value))
		{
			InputComponent->InputActionDelegateMap.Emplace(Iter.Key, FInputActionDelegate::FDelegate::CreateUObject(this, &ThisClass::ToggleWidget));
		}
	}
}

void UMBindWidgetByInputComponent::ToggleWidget(const FGameplayTag& Tag) const
{
	UMWidgetBase* Widget = WidgetInstanceMapWrapper->WidgetInstanceMap.FindRef(Tag);
	UGameInstance* GameInstance = GetGameInstance<UGameInstance>();
	UMViewportClient* ViewportClient = GameInstance ? Cast<UMViewportClient>(GameInstance->GetGameViewportClient()) : nullptr;

	if (IsValid(Widget) && IsValid(ViewportClient))
	{
		if (Widget->IsActivate())
		{
			ViewportClient->RemoveWidgetFromLayer(Tag);
		}
		else
		{
			ViewportClient->AddWidgetToLayer(Tag);
		}
	}
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
