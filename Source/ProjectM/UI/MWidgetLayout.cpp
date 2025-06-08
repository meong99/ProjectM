#include "MWidgetLayout.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "GameFramework/PlayerController.h"
#include "MWidgetBase.h"
#include "MWidgetLayer.h"
#include "Blueprint/WidgetTree.h"

UMWidgetLayout::UMWidgetLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UMWidgetLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MakeLayerById(GameLayer);
	MakeLayerById(IndependentLayer);
}

void UMWidgetLayout::NativeConstruct()
{
	Super::NativeConstruct();
	UMWidgetLayer* CurrentLayer = GetLayer((EMWidgetLayout)LayoutSwitcher->GetActiveWidgetIndex());
	if (CurrentLayer)
	{
		CurrentLayer->ActivateLayer();
	}
}

FReply UMWidgetLayout::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return FReply::Unhandled();
}

FReply UMWidgetLayout::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	return FReply::Unhandled();
}

void UMWidgetLayout::MakeLayerById(UMWidgetLayer* MainLayer)
{
	if (!MainLayer)
	{
		ensure(false);
		return;
	}

	for (int32 i = 0; i < (int32)EMWidgetLayerId::None; i++)
	{
		UOverlay* Overlay = WidgetTree->ConstructWidget<UOverlay>();
		MainLayer->AddChildToOverlay(Overlay);
	}
}

void UMWidgetLayout::ChangeWidgetLayer(EMWidgetLayout WidgetLayout) const
{
	LayoutSwitcher->SetActiveWidgetIndex((int32)WidgetLayout);
	UMWidgetLayer* CurrentLayer = GetLayer(WidgetLayout);
	if (CurrentLayer)
	{
		CurrentLayer->ActivateLayer();
	}
}

void UMWidgetLayout::ClearAllWidget()
{
	for (int32 i = 0; i < (int32)EMWidgetLayout::None; i++)
	{
		UMWidgetLayer* Layer = GetLayer((EMWidgetLayout)i);
		if (Layer)
		{
			Layer->ClearAllWidget();
		}
	}
}

void UMWidgetLayout::AddWidgetToCurrentLayer(UMWidgetBase* Widget) const
{
	AddWidgetToLayer(Widget, (EMWidgetLayout)LayoutSwitcher->ActiveWidgetIndex);
}

void UMWidgetLayout::RemoveWidgetToCurrentLayer(UMWidgetBase* Widget) const
{
	RemoveWidgetFromLayer(Widget, (EMWidgetLayout)LayoutSwitcher->ActiveWidgetIndex);
}

void UMWidgetLayout::AddWidgetToLayer(UMWidgetBase* Widget, EMWidgetLayout WidgetLayout) const
{
	UMWidgetLayer* CurrentLayout = GetLayer(WidgetLayout);
	if (CurrentLayout)
	{
		CurrentLayout->AddWidgetToLayer(Widget);
	}
	else
	{
		MCHAE_WARNING("CurrentLayout is null");
	}
}

void UMWidgetLayout::RemoveWidgetFromLayer(UMWidgetBase* Widget, EMWidgetLayout WidgetLayout) const
{
	UMWidgetLayer* CurrentLayout = GetLayer(WidgetLayout);
	if (CurrentLayout)
	{
		CurrentLayout->RemoveWidgetFromLayer(Widget);
	}
}

UMWidgetLayer* UMWidgetLayout::GetLayer(EMWidgetLayout WidgetLayout) const
{
	switch (WidgetLayout)
	{
		case EMWidgetLayout::GameLayer:
		{
			return GameLayer;
			break;
		}
		case EMWidgetLayout::IndependentLayer:
		{
			return IndependentLayer;
			break;
		}
		default:
		{
			break;
		}
	}

	ensure(false);
	MCHAE_WARNING("Can't found Widget Layout!");
	return nullptr;
}
