#include "MDefaultShopWidget.h"
#include "Components/Button.h"
#include "MDefaultShopDetailWidget.h"
#include "Interaction/MInteractiveAction_Base.h"
#include "Components/TextBlock.h"
#include "Interaction/MInteractiveAction_OpenShop.h"

UMDefaultShopWidget::UMDefaultShopWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMDefaultShopWidget::PreAddToLayer()
{
	ExitButton->OnClicked.AddDynamic(this, &UMDefaultShopWidget::OnClicked_ExitButton);
	UMInteractiveAction_OpenShop* ShopAction = Cast<UMInteractiveAction_OpenShop>(WidgetInfo.WidgetInstigator);
	if (ShopAction)
	{
		const FMShopDefinition& ShopDefinition = ShopAction->GetShopDefinition();
		ShopName->SetText(ShopDefinition.ShopName);
	}
}

void UMDefaultShopWidget::NativeConstruct()
{
	ShopDetail->SetWidgetInfo(WidgetInfo);
	UserInventoryDetail->SetWidgetInfo(WidgetInfo);
}

void UMDefaultShopWidget::OnClicked_ExitButton()
{
	UMInteractiveAction_Base* ActionBase = Cast<UMInteractiveAction_Base>(WidgetInfo.WidgetInstigator);
	if (ActionBase)
	{
		ActionBase->DeactivateAction();
	}
}
