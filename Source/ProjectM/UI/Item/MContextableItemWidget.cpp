#include "MContextableItemWidget.h"
#include "Components/Image.h"

UMContextableItemWidget::UMContextableItemWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMContextableItemWidget::SetDesiredSizeOverride(FVector2D DesiredSize)
{
	ItemImage->SetDesiredSizeOverride(DesiredSize);
}

void UMContextableItemWidget::SetBrushFromTexture(UTexture2D* Texture, bool bMatchSize /*= false*/)
{
	ItemImage->SetBrushFromTexture(Texture, bMatchSize);
}
