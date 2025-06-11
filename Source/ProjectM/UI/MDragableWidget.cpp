#include "MDragableWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Item/MContextableItemWidget.h"
#include "UI/MViewportClient.h"
#include "UI/Item/MItemContextWidget.h"
#include "PMGameplayTags.h"

UMDragableWidget::UMDragableWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMDragableWidget::NativeOnInitialized()
{
}

FReply UMDragableWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UMDragableWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	if (OutOperation)
	{
		OutOperation->Payload = this;
		OutOperation->DefaultDragVisual = ItemImage;
	}

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UMDragableWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

void UMDragableWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
}

void UMDragableWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (ItemRowId != INDEX_NONE)
	{
		EnableContextWidget();
	}
}

void UMDragableWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (ItemRowId != INDEX_NONE)
	{
		DisableContextWidget();
	}
}

void UMDragableWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (ItemRowId != INDEX_NONE)
	{
		DisableContextWidget();
	}
}

void UMDragableWidget::EnableContextWidget() const
{
	UMViewportClient* ViewportClient = UMViewportClient::Get(this);
	if (ViewportClient)
	{
		UMItemContextWidget* ContextWidget = Cast<UMItemContextWidget>(ViewportClient->AddWidgetToLayer(FPMGameplayTags::Get().UI_Registry_Game_ItemContext));
		if (ContextWidget)
		{
			ContextWidget->UpdateContextWidget(ItemRowId);
		}
	}
}

void UMDragableWidget::DisableContextWidget() const
{
	UMViewportClient* ViewportClient = UMViewportClient::Get(this);
	if (ViewportClient)
	{
		ViewportClient->RemoveWidgetFromLayer(FPMGameplayTags::Get().UI_Registry_Game_ItemContext);
	}
}
