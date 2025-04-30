#include "MPlayerTradeComponent.h"
#include "Inventory/PMInventoryManagerComponent.h"
#include "Character/Components/MWalletComponent.h"

UMPlayerTradeComponent::UMPlayerTradeComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

const FMTradeResponse UMPlayerTradeComponent::OnRequestSimpleDataGrant(const FMTradeRequest& Request)
{
	if (GetOwnerRole() != ENetRole::ROLE_Authority)
	{
		ensure(false);
		return MakeErrorResponse(Request, TEXT("Trading only working on authority! Don't call Client"));
	}

	AActor* Owner = GetOwner();
	UPMInventoryManagerComponent* InventoryManager = Owner->FindComponentByClass<UPMInventoryManagerComponent>();
	UMWalletComponent* WalletComponent = Owner->FindComponentByClass<UMWalletComponent>();

	if (!WalletComponent || !InventoryManager)
	{
		ensure(false);
		return MakeErrorResponse(Request, TEXT("Inventory or WalletComponent is not valid!"));
	}

	WalletComponent->AddGold(Request.GrantGold);
#pragma TODO("인벤 리팩토링 후 인벤 꽉찼을때 확인")
	for (const int32 ItemRowId : Request.GrantItems)
	{
		InventoryManager->AddItemtoInventory(ItemRowId);
	}

	return MakeSuccessResponse(Request);
}

const FMTradeResponse UMPlayerTradeComponent::OnRequestSimpleTrading(const FMTradeRequest& Request)
{
	if (GetOwnerRole() != ENetRole::ROLE_Authority)
	{
		ensure(false);
		return MakeErrorResponse(Request, TEXT("Trading only working on authority! Don't call Client"));
	}

	AActor* Owner = GetOwner();
	UPMInventoryManagerComponent* InventoryManager = nullptr;
	UMWalletComponent* WalletComponent = nullptr;

	if (Request.RequiredGold > 0)
	{
		WalletComponent = Owner->FindComponentByClass<UMWalletComponent>();
		if (!WalletComponent)
		{
			ensure(false);
			return MakeErrorResponse(Request, TEXT("Can't find WalletComponent!! Need WalletComponent for successfuly player trade!"));
		}

		if (Request.RequiredGold > WalletComponent->GetGold())
		{
			return MakeFailResponse(Request, TEXT("Not enough Gold"));
		}
	}

	if (Request.RequiredItems.Num() > 0)
	{
		InventoryManager = Owner ? Owner->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
		if (!InventoryManager)
		{
			ensure(false);
			return MakeErrorResponse(Request, TEXT("Can't find InventoryManager!! Need InventoryManager for successfuly player trade!"));
		}

		for (const auto& Iter : Request.RequiredItems)
		{
			if (Iter.Value > InventoryManager->GetItemQuantity(Iter.Value))
			{
				return MakeFailResponse(Request, TEXT("Not enough item quantity"));
			}
		}

		for (const auto& Iter : Request.RequiredItems)
		{
			InventoryManager->ChangeItemQuantity(Iter.Key, Iter.Value);
		}
	}

	WalletComponent->SubtractGold(Request.RequiredGold);
	WalletComponent->SubtractGold(Request.RequiredGold);

	return MakeSuccessResponse(Request);
}
