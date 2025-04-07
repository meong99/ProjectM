#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

struct FPMGameplayTags
{
	static const FPMGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();

	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddAllTags(UGameplayTagsManager& Manager);

	/*
	* Component Init State Tags
	*/
	FGameplayTag InitState_Spawned;
	FGameplayTag InitState_DataAvailable;
	FGameplayTag InitState_DataInitialized;
	FGameplayTag InitState_GameplayReady;

	/*
	* Input Tags
	*/
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Jump;
	FGameplayTag InputTag_Look_Mouse;

	/*
	* Widget Tags
	*/
	FGameplayTag UI_Registry_InputTag;
	FGameplayTag UI_Registry_InputTag_Inventory;
	FGameplayTag UI_Registry_Layout_DefaultLayout;
	FGameplayTag UI_Registry_HUD_MainHUD;

	FGameplayTag UI_Game_NameWidget;

	/*
	* Item
	*/
	FGameplayTag Item_Quentity;

private:
	static FPMGameplayTags GameplayTags;
};