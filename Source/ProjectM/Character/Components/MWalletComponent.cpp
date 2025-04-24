#include "MWalletComponent.h"

UMWalletComponent::UMWalletComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMWalletComponent::SubtractGold(int64 AdjustGold)
{
	if (AdjustGold > 0)
	{
		return;
	}
	int64 Temp = Gold;

	Gold += AdjustGold;
	if (Gold < 0)
	{
		Gold = 0;
	}
	else if (Gold > Temp)
	{
		Gold = Temp;
	}

	Delegate_OnChangeGold.Broadcast(AdjustGold, Gold);
}

void UMWalletComponent::AddGold(int64 AdjustGold)
{
	if (AdjustGold < 0)
	{
		return;
	}

	int64 Temp = Gold;

	Gold += AdjustGold;
	if (Gold < 0)
	{
		Gold = 0;
	}
	else if (Gold < Temp)
	{
		Gold = Temp;
	}

	Delegate_OnChangeGold.Broadcast(AdjustGold, Gold);
}

#if WITH_EDITOR
void UMWalletComponent::Debug_AddGold(int64 AdjustGold)
{
	AddGold(AdjustGold);
}

void UMWalletComponent::Debug_SubtractGold(int64 AdjustGold)
{
	SubtractGold(AdjustGold);
}
#endif