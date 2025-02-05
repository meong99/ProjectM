#include "MWidgetBase.h"
#include "Blueprint/DragDropOperation.h"
#include "MViewportClient.h"
#include "Engine/GameInstance.h"

UMWidgetBase::UMWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UMWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetIsFocusable(false);
}

void UMWidgetBase::AddWidgetToLayer(const int32 LayerId)
{
	UGameInstance* GameInstance = GetGameInstance<UGameInstance>();
	UMViewportClient* ViewportClient = GameInstance ? Cast<UMViewportClient>(GameInstance->GetGameViewportClient()) : nullptr;
	if (ViewportClient)
	{
		ViewportClient->AddWidgetToLayer(WidgetTag, LayerId);
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

// bool UMWidgetBase::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
// {
// 	if (InOperation)
// 	{
// // 		InOperation->OnDrop.Broadcast(InOperation);
// 	}
// 	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
// }
