#include "MInteractionActivity_ShowWidget.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "UI/MViewportClient.h"
#include "PMGameplayTags.h"
#include "MInteractionComponent.h"
#include "Character/Components/MNameWidgetComponent.h"
#include "UI/Interaction/MInteractionWidgetBase.h"

UE_DISABLE_OPTIMIZATION

UMInteractionActivity_ShowWidget::UMInteractionActivity_ShowWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMInteractionActivity_ShowWidget::InitAction(UMInteractionComponent* InOwner)
{
	Super::InitAction(InOwner);
}

void UMInteractionActivity_ShowWidget::ActivateAction()
{
	UMViewportClient* ViewportClient = Cast<UMViewportClient>(GetWorld()->GetGameInstance()->GetGameViewportClient());

	if (ViewportClient && Owner.IsValid())
	{
		FGameplayTag NameWidgetTag = FPMGameplayTags::Get().UI_Game_NameWidget;
		for (const FGameplayTag& Tag : WidgetTags)
		{
			if (Tag == NameWidgetTag)
			{
				EnableNameWidget();
			}
			else
			{
				ViewportClient->AddWidgetToLayer(Tag, 0, Owner->GetOwner());
			}
		}
	}
}

void UMInteractionActivity_ShowWidget::DeactivateAction()
{
	UMViewportClient* ViewportClient = Cast<UMViewportClient>(GetWorld()->GetGameInstance()->GetGameViewportClient());
	if (ViewportClient)
	{
		FGameplayTag NameWidgetTag = FPMGameplayTags::Get().UI_Game_NameWidget;
		for (const FGameplayTag& Tag : WidgetTags)
		{
			if (Tag == NameWidgetTag)
			{
				DisableNameWidget();
			}
			else
			{
				ViewportClient->RemoveWidgetFromLayer(Tag);
			}
		}
	}
}

void UMInteractionActivity_ShowWidget::EnableNameWidget() const
{
	UMNameWidgetComponent* WidgetComp = GetNameWidgetComponent();
	if (WidgetComp)
	{
		WidgetComp->EnableNameWidget();
	}
}

void UMInteractionActivity_ShowWidget::DisableNameWidget() const
{
	UMNameWidgetComponent* WidgetComp = GetNameWidgetComponent();
	if (WidgetComp)
	{
		WidgetComp->DisableNameWidget();
	}
}

UMNameWidgetComponent* UMInteractionActivity_ShowWidget::GetNameWidgetComponent() const
{
	if (Owner.IsValid() && Owner->GetOwner())
	{
		return Owner->GetOwner()->FindComponentByClass<UMNameWidgetComponent>();
	}

	return nullptr;
}

UE_ENABLE_OPTIMIZATION