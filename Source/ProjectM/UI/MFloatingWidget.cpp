#include "MFloatingWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/GameInstance.h"

UMFloatingWidget::UMFloatingWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMFloatingWidget::NativeOnInitialized()
{
}

void UMFloatingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (bIsFloating && TargetWidget)
	{
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);
		if (CanvasSlot)
		{
			double x;
			double y;
			UWidgetLayoutLibrary::GetMousePositionScaledByDPI(GetOwningPlayer(), x, y);
			FVector2D MousePosition{x, y};
			FVector2D NewPosition = MousePosition + DragOffset;
			NewPosition *= 0.8;
			CanvasSlot->SetPosition(NewPosition);
		}
	}
}

FReply UMFloatingWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
// 	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
// 	{
// 		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
// 	}

	if (TargetWidget == nullptr)
	{
		UWidgetTree* OuterWidgetTree = Cast<UWidgetTree>(GetOuter());

		if (OuterWidgetTree)
		{
			TargetWidget = OuterWidgetTree->FindWidget<UWidget>(TEXT("FloatingTarget"));
		}
	}

	if (TargetWidget)
	{
		bIsFloating = true;
		FVector2d MousePosition = InMouseEvent.GetScreenSpacePosition();
		FVector2d WidgetPosition = TargetWidget->GetCachedGeometry().GetAbsolutePosition();

		DragOffset = WidgetPosition - MousePosition;
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UMFloatingWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsFloating = false;
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UMFloatingWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
// 	if (bIsFloating && TargetWidget)
// 	{
// 		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);
// 		if (CanvasSlot)
// 		{
// 			FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
// 			MousePosition = InGeometry.AbsoluteToLocal(MousePosition);
// 			FVector2D NewPosition = MousePosition + DragOffset;
// 			CanvasSlot->
// 
// // 			float scale = UWidgetLayoutLibrary::GetViewportScale(GetGameInstance()->GetGameViewportClient());
// // 			NewPosition *= scale;
// 
// 			CanvasSlot->SetPosition(NewPosition);
// 		}
// 	}
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}
