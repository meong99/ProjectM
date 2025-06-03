#include "MWidgetLayer.h"
#include "Components/OverlaySlot.h"

UMWidgetLayer::UMWidgetLayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMWidgetLayer::AddWidgetToLayer(UMWidgetBase* Widget)
{
	if (Widget == nullptr || Widget->IsInLayer())
	{
		MCHAE_WARNING("Widget Instance is null or already in layer");
		return;
	}

	UOverlaySlot* OverlaySlot = AddChildToOverlay(Widget);
	if (OverlaySlot)
	{
		OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

		Widget->SetActivate(true);
		if (Widget->IsFocusable())
		{
			Widget->SetFocus();
		}

		EMWidgetInputMode InputMode = Widget->GetInputMode();
		PushInputMode(InputMode, Widget->GetWidgetTag());
		SetInputMode(Widget);
	}
	else
	{
		MCHAE_WARNING("OverlaySlot is null");
	}
}

void UMWidgetLayer::RemoveWidgetFromLayer(UMWidgetBase* Widget)
{
	if (Widget == nullptr)
	{
		ensure(false);
		MCHAE_WARNING("Widget Instance is null!");
		return;
	}

	RemoveChild(Widget);
	Widget->SetActivate(false);
	EMWidgetInputMode InputMode = Widget->GetInputMode();
	PopInputMode(InputMode);
	SetInputMode();
}

void UMWidgetLayer::ActivateLayer()
{
	SetInputMode();
	OldTopInputMode = CurrentTopInputMode;
	CurrentTopInputMode = GetTopInputMode();
}

void UMWidgetLayer::ClearAllWidget()
{
	ClearChildren();
}

void UMWidgetLayer::SetInputMode(UMWidgetBase* Widget) const
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return;
	}

	if (OldTopInputMode == CurrentTopInputMode)
	{
		return;
	}

	switch (CurrentTopInputMode)
	{
		case EMWidgetInputMode::GameOnly:
		{
			FInputModeGameOnly NewMode;
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(NewMode);
			break;
		}
		case EMWidgetInputMode::GameAndUIWithShowMouse:
		{
			FInputModeGameAndUI NewMode;
			NewMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			PlayerController->SetInputMode(NewMode);
			PlayerController->bShowMouseCursor = true;
			break;
		}
		case EMWidgetInputMode::UIOnly:
		{
			FInputModeUIOnly NewMode;
			NewMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			PlayerController->SetInputMode(NewMode);
			PlayerController->FlushPressedKeys();
			PlayerController->bShowMouseCursor = true;
			break;
		}
		default:
		{
			break;
		}
	}
}

void UMWidgetLayer::PushInputMode(EMWidgetInputMode Input, const FGameplayTag& Tag)
{
	if (Tag.IsValid())
	{
		TStack<FGameplayTag>& WidgetStack = WidgetMapByInputMode.FindOrAdd(Input);
		WidgetStack.Push(Tag);

		if (CurrentTopInputMode < Input)
		{
			OldTopInputMode = CurrentTopInputMode;
			CurrentTopInputMode = Input;
		}
	}
}

void UMWidgetLayer::PopInputMode(EMWidgetInputMode Input)
{
	TStack<FGameplayTag>& WidgetStack = WidgetMapByInputMode.FindOrAdd(Input);
	if (!WidgetStack.IsEmpty())
	{
		WidgetStack.Pop();
		OldTopInputMode = CurrentTopInputMode;
		CurrentTopInputMode = GetTopInputMode();
	}
}

EMWidgetInputMode UMWidgetLayer::GetTopInputMode(EMWidgetInputMode Input) const
{
	EMWidgetInputMode InputMode = Input;

	if (!WidgetMapByInputMode.FindRef(EMWidgetInputMode::GameAndUIWithShowMouse).IsEmpty())
	{
		InputMode = EMWidgetInputMode::GameAndUIWithShowMouse;
	}

	if (!WidgetMapByInputMode.FindRef(EMWidgetInputMode::UIOnly).IsEmpty())
	{
		InputMode = EMWidgetInputMode::UIOnly;
	}

	return InputMode;
}
