#include "MDefaultShopWidget.h"
#include "Components/Button.h"
#include "MDefaultShopDetailWidget.h"
#include "Interaction/MInteractionActivity_Base.h"

UMDefaultShopWidget::UMDefaultShopWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMDefaultShopWidget::PreAddToLayer()
{
	ExitButton->OnClicked.AddDynamic(this, &UMDefaultShopWidget::OnClicked_ExitButton);

	ShopDetail->SetWidgetInstigator(WidgetInstigator);
	UserInventoryDetail->SetWidgetInstigator(WidgetInstigator);
	ShopDetail->PreAddToLayer();
	UserInventoryDetail->PreAddToLayer();
}

void UMDefaultShopWidget::OnClicked_ExitButton()
{
	UMInteractionActivity_Base* ActionBase = Cast<UMInteractionActivity_Base>(WidgetInstigator);
	if (ActionBase)
	{
		ActionBase->DeactivateAction();
	}
}
