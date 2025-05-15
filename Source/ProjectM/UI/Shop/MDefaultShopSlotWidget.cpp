#include "MDefaultShopSlotWidget.h"
#include "Inventory/PMInventoryItemDefinition.h"
#include "Engine/Engine.h"
#include "System/MDataTableManager.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "Components/MTradeManager.h"
#include "GameFramework/Actor.h"
#include "Components/MTradeComponentBase.h"
#include "Character/Components/MWalletComponent.h"
#include "Components/WidgetSwitcher.h"

UMDefaultShopSlotWidget::UMDefaultShopSlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FReply UMDefaultShopSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	OnClickItem();

	return FReply::Handled();
}

void UMDefaultShopSlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (PlayerWalletComp.IsValid() && ItemCDO)
	{
		if (PlayerWalletComp->GetGold() < ItemCDO->BuyPrice)
		{
			PriceWidgetSwitcher->SetActiveWidget(NotEnoughPrice);
		}
		else
		{
			PriceWidgetSwitcher->SetActiveWidget(Price);
		}
	}
}

void UMDefaultShopSlotWidget::InitSlot(const int32 InRowId)
{
	RowId = InRowId;

	UMDataTableManager* TableManager = GEngine->GetEngineSubsystem<UMDataTableManager>();
	if (TableManager)
	{
		ItemCDO = TableManager->GetItemDefinition(RowId);
		if (ItemCDO)
		{
			ItemIcon->SetBrushFromTexture(ItemCDO->ItemIcon);
// 			ItemIcon->Brush.SetImageSize(100.f, 100.f);
			ItemName->SetText(ItemCDO->DisplayName);
			SetPrice(ItemCDO->BuyPrice);
		}
		else
		{
			ensure(false);
			SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		ensure(false);
		SetVisibility(ESlateVisibility::Collapsed);
	}

	APlayerController* PlayerController = GetOwningPlayer();
	if (PlayerController)
	{
		PlayerWalletComp = PlayerController->FindComponentByClass<UMWalletComponent>();
	}
}

void UMDefaultShopSlotWidget::OnClickItem()
{
	APlayerController* PlayerController = GetOwningPlayer();
	UMTradeComponentBase* PlayerTradeComponent = PlayerController ? PlayerController->FindComponentByClass<UMTradeComponentBase>() : nullptr;
	UMTradeManager* TradeManager = GetWorld()->GetGameState()->FindComponentByClass<UMTradeManager>();
	AActor* ShopNpc = Cast<AActor>(WidgetInfo.WidgetOwnerActor);
	if (!ShopNpc || !TradeManager || !PlayerTradeComponent)
	{
		ensure(false);
		return ;
	}

	if (PlayerWalletComp.IsValid() && PlayerWalletComp->GetGold() < ItemCDO->BuyPrice)
	{
		MCHAE_LOG("Not enough money");
		return;
	}

	FMTradeRequest Request;
	Request.RequestType = EMRequestType::Trade;
	Request.RequiredGold = ItemCDO->BuyPrice;
	Request.GrantItems.Add({RowId, 1});

	PlayerTradeComponent->Server_OnRequestSimpleTrading(ShopNpc, Request);
}

void UMDefaultShopSlotWidget::SetPrice(int32 BuyPrice)
{
	Price->SetText(FText::AsNumber(ItemCDO->BuyPrice));
	NotEnoughPrice->SetText(FText::AsNumber(ItemCDO->BuyPrice));
}
