#include "MWidgetLayout.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

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
