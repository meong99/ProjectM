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
#pragma TODO("인벤 리팩토링 후 인벤 꽉찼을때 확인")
	for (const int32 ItemRowId : Request.GrantItems.ItemRowIds)
	{
#pragma TODO("아이템 갯수 한번에 추가 가능하도록 리팩토링")
		InventoryManager->AddItemtoInventory(ItemRowId);
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

	if (Request.RequiredItems.ItemRowIds.Num() > 0)
	{
		for (int32 i = 0; i < Request.RequiredItems.ItemRowIds.Num(); i++)
		{
			int32 RequirementNum = Request.RequiredItems.ItemCount.IsValidIndex(i) ? Request.RequiredItems.ItemCount[i] : 1;
			if (RequirementNum > InventoryManager->GetItemQuantity(Request.RequiredItems.ItemRowIds[i]))
			{
				return;
			}
		}

		for (int32 i = 0; i < Request.RequiredItems.ItemRowIds.Num(); i++)
		{
			int32 RequirementNum = Request.RequiredItems.ItemCount.IsValidIndex(i) ? Request.RequiredItems.ItemCount[i] : 1;
			InventoryManager->ChangeItemQuantity(Request.RequiredItems.ItemRowIds[i], -RequirementNum);
		}
	}

	WalletComponent->AddGold(Request.GrantGold);
	for (int32 i = 0; i < Request.GrantItems.ItemRowIds.Num(); i++)
	{
		int32 RequirementNum = Request.GrantItems.ItemCount.IsValidIndex(i) ? Request.GrantItems.ItemCount[i] : 1;
		InventoryManager->AddItemtoInventory(Request.GrantItems.ItemRowIds[i]);
	}
}
