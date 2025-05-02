#include "MDefaultShopDetailWidget.h"
#include "Components/VerticalBox.h"
#include "MDefaultShopSlotWidget.h"

UMDefaultShopDetailWidget::UMDefaultShopDetailWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMDefaultShopDetailWidget::PreAddToLayer()
{
	if (Type == EMShopDetailType::Shop)
	{
		InitShopDetail();
	}

	if ((ItemRowIdArray.Num() == 0 && Type == EMShopDetailType::Shop) || !SlotClass)
	{
		ensure(false);
	}
}

void UMDefaultShopDetailWidget::InitShopDetail()
{
	for (const int32 RowId : ItemRowIdArray)
	{
		UMDefaultShopSlotWidget* SlotWidget = CreateWidget<UMDefaultShopSlotWidget>(GetOwningPlayer(), SlotClass);
		if (!SlotWidget)
		{
			ensure(false);
			continue;
		}
		SlotWidget->SetWidgetInstigator(WidgetInstigator);
		SlotWidget->InitSlot(RowId);
		SlotVerticalBox->AddChildToVerticalBox(SlotWidget);
	}
}
