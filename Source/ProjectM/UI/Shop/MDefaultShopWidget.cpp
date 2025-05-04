#include "MDefaultShopWidget.h"
#include "Components/Button.h"
#include "MDefaultShopDetailWidget.h"
#include "Interaction/MInteractiveAction_Base.h"

UMDefaultShopWidget::UMDefaultShopWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMDefaultShopWidget::PreAddToLayer()
{
	Super::PreAddToLayer();
	ExitButton->OnClicked.AddDynamic(this, &UMDefaultShopWidget::OnClicked_ExitButton);

	ShopDetail->SetWidgetInfo(WidgetInfo);
	UserInventoryDetail->SetWidgetInfo(WidgetInfo);
	ShopDetail->PreAddToLayer();
	UserInventoryDetail->PreAddToLayer();
}

void UMDefaultShopWidget::OnClicked_ExitButton()
{
	UMInteractiveAction_Base* ActionBase = Cast<UMInteractiveAction_Base>(WidgetInfo.WidgetInstigator);
	if (ActionBase)
	{
		ActionBase->DeactivateAction();
	}
}
