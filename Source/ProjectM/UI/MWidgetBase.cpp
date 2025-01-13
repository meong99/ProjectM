#include "MWidgetBase.h"
#include "Blueprint/DragDropOperation.h"

UMWidgetBase::UMWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UMWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

// bool UMWidgetBase::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
// {
// 	if (InOperation)
// 	{
// // 		InOperation->OnDrop.Broadcast(InOperation);
// 	}
// 	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
// }
