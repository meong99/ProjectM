#include "MPlayerTradeComponent.h"
#include "Inventory/PMInventoryManagerComponent.h"
#include "Character/Components/MWalletComponent.h"

UMPlayerTradeComponent::UMPlayerTradeComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMPlayerTradeComponent::Server_OnRequestSimpleDataGrant_Implementation(AActor* Requestor, const FMTradeRequest& Request)
{
	if (GetOwnerRole() != ENetRole::ROLE_Authority)
	{
		ensure(false);
		return ;
	}

	AActor* Owner = GetOwner();
	UPMInventoryManagerComponent* InventoryManager = Owner->FindComponentByClass<UPMInventoryManagerComponent>();
	UMWalletComponent* WalletComponent = Owner->FindComponentByClass<UMWalletComponent>();

	if (!WalletComponent || !InventoryManager)
	{
		ensure(false);
		return;
	}

	WalletComponent->AddGold(Request.GrantGold);
	for (int32 i = 0; i < Request.GrantItems.Num(); i++)
	{
		int32 GrantItemNum = Request.GrantItems[i].ItemCount;

		FMItemRequest ItemRequest;
		ItemRequest.SetItemRequest(EMItemRequestType::AddItem, Request.GrantItems[i].ItemRowId, GrantItemNum);

		InventoryManager->RequestItemToInventory(ItemRequest);
	}
}

void UMPlayerTradeComponent::Server_OnRequestSimpleTrading_Implementation(AActor* Requestor, const FMTradeRequest& Request)
{
	if (GetOwnerRole() != ENetRole::ROLE_Authority)
	{
		ensure(false);
		return;
	}

	AActor* Owner = GetOwner();
	UPMInventoryManagerComponent* InventoryManager = Owner ? Owner->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
	UMWalletComponent* WalletComponent = Owner ? Owner->FindComponentByClass<UMWalletComponent>() : nullptr;
	if (!InventoryManager || !WalletComponent)
	{
		ensure(false);
		return;
	}

	if (Request.RequiredGold > 0)
	{
		if (Request.RequiredGold > WalletComponent->GetGold())
		{
			return;
		}

		WalletComponent->SubtractGold(Request.RequiredGold);
	}

	if (Request.RequiredItems.Num() > 0)
	{
		for (int32 i = 0; i < Request.RequiredItems.Num(); i++)
		{
			int32 RequirementNum = Request.RequiredItems[i].ItemCount;
			if (RequirementNum > InventoryManager->GetItemQuantity(Request.RequiredItems[i].ItemRowId))
			{
				return;
			}
		}

		for (int32 i = 0; i < Request.RequiredItems.Num(); i++)
		{
			int32 RequirementNum = Request.RequiredItems[i].ItemCount;
			FMItemRequest ItemRequest;
			ItemRequest.SetItemRequest(EMItemRequestType::RemoveItem, Request.RequiredItems[i].ItemRowId, -RequirementNum);
			InventoryManager->RequestItemToInventory(ItemRequest);
		}
	}

	WalletComponent->AddGold(Request.GrantGold);
	for (int32 i = 0; i < Request.GrantItems.Num(); i++)
	{
		int32 GrantItemNum = Request.GrantItems[i].ItemCount;

		FMItemRequest ItemRequest;
		ItemRequest.SetItemRequest(EMItemRequestType::AddItem, Request.GrantItems[i].ItemRowId, GrantItemNum);
		InventoryManager->RequestItemToInventory(ItemRequest);
	}
}
