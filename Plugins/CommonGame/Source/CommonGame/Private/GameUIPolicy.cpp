// Fill out your copyright notice in the Description page of Project Settings.

#include "GameUIPolicy.h"
#include "CommonLocalPlayer.h"
#include "PrimaryGameLayout.h"

UPrimaryGameLayout* UGameUIPolicy::GetRootLayout(const UCommonLocalPlayer* LocalPlayer) const
{
	const FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer);
	return LayoutInfo ? LayoutInfo->RootLayout : nullptr;
}

TSubclassOf<UPrimaryGameLayout> UGameUIPolicy::GetLayoutWidgetClass(UCommonLocalPlayer* LocalPlayer)
{
	return LayoutClass.LoadSynchronous();
}

void UGameUIPolicy::CreateLayoutWidget(UCommonLocalPlayer* LocalPlayer)
{
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UPrimaryGameLayout> LayoutWidgetClass = GetLayoutWidgetClass(LocalPlayer);
		if (ensure(LayoutWidgetClass && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract)))
		{
			UPrimaryGameLayout* NewLayoutObject = CreateWidget<UPrimaryGameLayout>(PlayerController, LayoutWidgetClass);

			RootViewportLayouts.Emplace(LocalPlayer, NewLayoutObject, true);

			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
}

void UGameUIPolicy::AddLayoutToViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	// CommonUserWidget에 PlayerContext를 설정 (Player와 Layout 매칭)
	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));

	// 해당 Layout의 우선순위를 아주 높게 만들면서 Widget을 붙인다.
	Layout->AddToPlayerScreen(1000);
}

void UGameUIPolicy::RemoveLayoutFromViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	// UCommonUserWidget의 SlateWidget을 가져와서 Parent와의 연결고리를 끊는다. 
	// Layout을 더 이상 사용하지 않는다. ( 위젯의 제거가 이런 식으로 되는건가? )
	TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
	if (LayoutSlateWidget.IsValid())
	{
		Layout->RemoveFromParent();
	}
}

void UGameUIPolicy::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	LocalPlayer->OnPlayerControllerSet.AddWeakLambda(this, [this](UCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController)
		{
			NotifyPlayerRemoved(LocalPlayer);

			if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
			{
				AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
				LayoutInfo->bAddedToViewport = true;
			}
			else
			{
				CreateLayoutWidget(LocalPlayer);
			}
		});

	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bAddedToViewport = true;
	}
	else
	{
		CreateLayoutWidget(LocalPlayer);
	}
}

void UGameUIPolicy::NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer)
{
	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		RemoveLayoutFromViewport(LocalPlayer, LayoutInfo->RootLayout);

		// 비활성화 확인 용도
		LayoutInfo->bAddedToViewport = false;
	}
}

void UGameUIPolicy::NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);

	LocalPlayer->OnPlayerControllerSet.RemoveAll(this);

	const int32 LayoutInfoIdx = RootViewportLayouts.IndexOfByKey(LocalPlayer);
	if (LayoutInfoIdx != INDEX_NONE)
	{
		// 만약 PrimaryGameLayout이 있으면 Viewport에서도 제거한다.
		UPrimaryGameLayout* Layout = RootViewportLayouts[LayoutInfoIdx].RootLayout;
		RootViewportLayouts.RemoveAt(LayoutInfoIdx);
		RemoveLayoutFromViewport(LocalPlayer, Layout);
	}
}
