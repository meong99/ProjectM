#include "PMGameplayTags.h"
#include "GameplayTagsManager.h"

FPMGameplayTags FPMGameplayTags::GameplayTags;

void FPMGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	GameplayTags.AddAllTags(Manager);
	// 태그를 ini로 Export하는 코드가 필요하다.
	// 그렇지 않으면 바이너리로 에디터를 열었을 때 런타임에 추가한 태그는 적용되지 않는다.
}

void FPMGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(ProjectMNativeTags) ")) + FString(TagComment));
}

void FPMGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	/*
	* Component Init State Tags
	*/
	AddTag(InitState_Spawned, "InitState.Spawned", "1: Actor/Component has initially spawned and can be extended");
	AddTag(InitState_DataAvailable, "InitState.DataAvailable", "2: All Required data has been loaded/replicated and is ready for initialize");
	AddTag(InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initialized for this actor/component");
	AddTag(InitState_GameplayReady, "InitState.GameplayReady", "4: The actor/component is fully ready for active gameplay");


	/*
	* Input Tags
	*/
	AddTag(InputTag_Move, "InputTag.Move", "");
	AddTag(InputTag_Look_Mouse, "InputTag.Look.Mouse", "");

	/*
	* Input Tags
	*/
	AddTag(HUD_Slot_Inventory, "HUD.Slot.Inventory", "");
}