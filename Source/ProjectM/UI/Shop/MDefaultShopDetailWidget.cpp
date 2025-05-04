#include "MDefaultShopDetailWidget.h"
#include "Components/VerticalBox.h"
#include "MDefaultShopSlotWidget.h"
#include "Interaction/MInteractiveAction_OpenShop.h"

UMDefaultShopDetailWidget::UMDefaultShopDetailWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMDefaultShopDetailWidget::PreAddToLayer()
{
	Super::PreAddToLayer();

	UMInteractiveAction_OpenShop* ShopAction = Cast<UMInteractiveAction_OpenShop>(WidgetInfo.WidgetInstigator);
	if (!ShopAction)
	{
		ensure(false);
		return;
	}
	ShopDefinition = ShopAction->GetShopDefinition();
	if ((ShopDefinition.ShopItemRowids.Num() == 0 && Type == EMShopDetailType::Shop) || !ShopDefinition.SlotClass)
	{
		ensure(false);
	}

	if (Type == EMShopDetailType::Shop)
	{
		InitShopDetail();
	}
}

void UMDefaultShopDetailWidget::InitShopDetail()
{
	for (const int32 RowId : ShopDefinition.ShopItemRowids)
	{
		UMDefaultShopSlotWidget* SlotWidget = CreateWidget<UMDefaultShopSlotWidget>(GetOwningPlayer(), ShopDefinition.SlotClass);
		if (!SlotWidget)
		{
			ensure(false);
			continue;
		}
		SlotWidget->SetWidgetInfo(WidgetInfo);
		SlotWidget->InitSlot(RowId);
		SlotVerticalBox->AddChildToVerticalBox(SlotWidget);
	}
}
