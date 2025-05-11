#include "MInventoryToastWidget.h"
#include "GameModes/PMExperienceManagerComponent.h"
#include "Inventory/PMInventoryManagerComponent.h"
#include "Character/Components/MWalletComponent.h"
#include "GameModes/PMExperienceDefinition.h"
#include "Inventory/MInventoryTypes.h"

UMInventoryToastWidget::UMInventoryToastWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMInventoryToastWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UPMExperienceManagerComponent* ExperienceManager = GetWorld()->GetGameState()->FindComponentByClass<UPMExperienceManagerComponent>();
	if (ExperienceManager)
	{
		ExperienceManager->CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate::CreateUObject(this, &UMInventoryToastWidget::OnExperienceLoaded));
	}
}

void UMInventoryToastWidget::InitThisWidget()
{
	APlayerController* Controller = GetOwningPlayer();
	InvenManager = Controller ? Controller->FindComponentByClass<UPMInventoryManagerComponent>() : nullptr;
	if (InvenManager)
	{
		InvenManager->Delegate_OnNewItemAdded.AddUObject(this, &ThisClass::OnNotifyNewItem);
	}
	WalletComponent = Controller ? Controller->FindComponentByClass<UMWalletComponent>() : nullptr;
	if (WalletComponent)
	{
		WalletComponent->Delegate_OnChangeGold.AddDynamic(this, &ThisClass::OnChangeGold);
	}
}

void UMInventoryToastWidget::OnChangeGold(int64 AdjustPrice, int64 NewGold)
{
	K2_OnChangeGold(AdjustPrice, NewGold);
}

void UMInventoryToastWidget::OnNotifyNewItem(const FPMInventoryEntry& ItemEntry, const FMItemResponse& ItemRespons)
{
	if (ItemRespons.ItemRequest.RequestType == EMItemRequestType::AddItem)
	{
		K2_NotifyNewItem(ItemEntry);
	}
}

void UMInventoryToastWidget::OnExperienceLoaded(const UPMExperienceDefinition* LoadedExperienceDefinition)
{
	InitThisWidget();
}
