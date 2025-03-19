#include "MWidgetLayout.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "GameFramework/PlayerController.h"
#include "MWidgetBase.h"

UMWidgetLayout::UMWidgetLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UMWidgetLayout::ChangeWidgetLayout(EMWidgetLayout WidgetLayout) const
{
	LayoutSwitcher->SetActiveWidgetIndex((int32)WidgetLayout);
}

void UMWidgetLayout::AddWidgetToCurrentLayout(UMWidgetBase* Widget) const
{
	UOverlay* CurrentLayout = GetLayout((EMWidgetLayout)LayoutSwitcher->ActiveWidgetIndex);
	if (CurrentLayout)
	{
		CurrentLayout->AddChild(Widget);
	}
}

void UMWidgetLayout::RemoveWidgetToCurrentLayout(UMWidgetBase* Widget) const
{
	UOverlay* CurrentLayout = GetLayout((EMWidgetLayout)LayoutSwitcher->ActiveWidgetIndex);
	if (CurrentLayout)
	{
		CurrentLayout->RemoveChild(Widget);
	}
}

void UMWidgetLayout::AddWidgetToLayout(UMWidgetBase* Widget, EMWidgetLayout WidgetLayout) const
{
	if (Widget == nullptr)
	{
		MCHAE_WARNING("Widget Instance is null!");
		return;
	}

	UOverlay* CurrentLayout = GetLayout(WidgetLayout);
	if (CurrentLayout)
	{
		UOverlaySlot* OverlaySlot = CurrentLayout->AddChildToOverlay(Widget);
		if (OverlaySlot)
		{
			OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

			SetInputMode((uint8)Widget->GetInputMode());
		}
	}
}

void UMWidgetLayout::RemoveWidgetFromLayout(UMWidgetBase* Widget, EMWidgetLayout WidgetLayout) const
{
	if (Widget == nullptr)
	{
		MCHAE_WARNING("Widget Instance is null!");
		return;
	}

	UOverlay* CurrentLayout = GetLayout(WidgetLayout);
	if (CurrentLayout)
	{
		CurrentLayout->RemoveChild(Widget);
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
#pragma TODO("모든 마우스가 보이는 위젯이 꺼져야 동작해야함 이거 메멘토 패턴으로 되돌리기 하면 좋을 듯")
		FInputModeGameOnly NewMode;
		PlayerController->SetInputMode(NewMode);
		PlayerController->bShowMouseCursor = false;
	}
}

void UMWidgetLayout::SetInputMode(uint8 NewInputMode) const
{
	EMWidgetInputMode MyInputMode = (EMWidgetInputMode)NewInputMode;

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController == nullptr)
	{
		return ;
	}

	switch (MyInputMode)
	{
		case EMWidgetInputMode::GameAndUI :
		{
			FInputModeGameAndUI NewMode;
			PlayerController->SetInputMode(NewMode);
			break;
		}
		case EMWidgetInputMode::GameOnly :
		{
			FInputModeGameOnly NewMode;
			PlayerController->SetInputMode(NewMode);
			break;
		}
		case EMWidgetInputMode::UIOnly :
		{
			FInputModeUIOnly NewMode;
			PlayerController->SetInputMode(NewMode);
			PlayerController->bShowMouseCursor = true;
			break;
		}
		case EMWidgetInputMode::GameAndUIWithShowMouse:
		{
			FInputModeGameAndUI NewMode;
			PlayerController->SetInputMode(NewMode);
			PlayerController->bShowMouseCursor = true;
			break;
		}
		default:
		{
			break;
		}
	}
}

UOverlay* UMWidgetLayout::GetLayout(EMWidgetLayout WidgetLayout) const
{
	switch (WidgetLayout)
	{
		case EMWidgetLayout::GameLayout:
		{
			return GameLayout;
			break;
		}
		case EMWidgetLayout::MenuLayout :
		{
			return MenuLayout;
			break;
		}
		case EMWidgetLayout::ShopLayout :
		{
			return ShopLayout;
			break;
		}
		case EMWidgetLayout::OtherLayout :
		{
			return OtherLayout;
			break;
		}
		default:
		{
			break;
		}
	}

	MCHAE_WARNING("Can't found Widget Layout!");
	return nullptr;
}
