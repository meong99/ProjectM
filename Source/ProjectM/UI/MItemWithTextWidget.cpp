#include "MItemWithTextWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UMItemWithTextWidget::UMItemWithTextWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMItemWithTextWidget::SetItemTexture(UTexture2D* ItemTexture)
{
	ItemImage->SetBrushFromTexture(ItemTexture);
}

void UMItemWithTextWidget::SetItemContextText(const FText& ItemContext)
{
	ItemContextText->SetText(ItemContext);
}
