#include "MWidgetBase.h"
#include "Blueprint/DragDropOperation.h"
#include "MViewportClient.h"
#include "Engine/GameInstance.h"
#include "Player/PMPlayerControllerBase.h"
#include "Blueprint/WidgetTree.h"

UMWidgetBase::UMWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UMWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetIsFocusable(false);
}

FReply UMWidgetBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return FReply::Handled();
}

FReply UMWidgetBase::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	return FReply::Handled();
}

void UMWidgetBase::AddWidgetToLayer(const FMWidgetInfo& InWidgetInfo, const int32 LayerId)
{
	UGameInstance* GameInstance = GetGameInstance<UGameInstance>();
	UMViewportClient* ViewportClient = GameInstance ? Cast<UMViewportClient>(GameInstance->GetGameViewportClient()) : nullptr;
	if (ViewportClient)
	{
		ViewportClient->AddWidgetToLayer(WidgetTag, InWidgetInfo, LayerId);
	}
}

void UMWidgetBase::RemoveWidgetFromLayer(const int32 LayerId)
{
	UGameInstance* GameInstance = GetGameInstance<UGameInstance>();
	UMViewportClient* ViewportClient = GameInstance ? Cast<UMViewportClient>(GameInstance->GetGameViewportClient()) : nullptr;
	if (ViewportClient)
	{
		ViewportClient->RemoveWidgetFromLayer(WidgetTag, LayerId);
	}
}

void UMWidgetBase::CallPreAddToLayer()
{
	PreAddToLayer();
	SetInitialized(true);

	TArray<UWidget*> AllWidgets;
	WidgetTree->GetAllWidgets(AllWidgets);

	for (UWidget* Widget : AllWidgets)
	{
		UMWidgetBase* MWidget = Cast<UMWidgetBase>(Widget);

		if (MWidget)
		{
			MWidget->PreAddToLayer();
			MWidget->SetInitialized(true);
		}
	}
}

void UMWidgetBase::PreAddToLayer()
{
	bIsInitialized = true;
}

APMPlayerControllerBase* UMWidgetBase::GetPlayerController() const
{
	return GetOwningPlayer<APMPlayerControllerBase>();
}

// bool UMWidgetBase::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
// {
// 	if (InOperation)
// 	{
// // 		InOperation->OnDrop.Broadcast(InOperation);
// 	}
// 	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
// }
