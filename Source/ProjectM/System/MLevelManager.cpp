#include "MLevelManager.h"
#include "GameModes/PMUserFacingExperienceDefinition.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "CommonSessionSubSystem.h"
#include "Engine/LocalPlayer.h"

UMLevelManager::UMLevelManager()
{
}

void UMLevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UMLevelManager::TravelLevel(UPMUserFacingExperienceDefinition* UFED, const FString& Ip)
{
	if (UFED)
	{
		UCommonSessionSubSystem* Session = GetSessionSubsystem();
		if (Session)
		{
 			Session->HostSession(GetPlayerController(), UFED->CreateHostingRequst(Ip));
		}
	}
}

UCommonSessionSubSystem* UMLevelManager::GetSessionSubsystem() const
{
	UWorld* World = GetWorld();
	UGameInstance* GameInstance = World ? World->GetGameInstance() : nullptr;
	UCommonSessionSubSystem* Session = GameInstance ? GameInstance->GetSubsystem<UCommonSessionSubSystem>() : nullptr;

	return Session;
}

APlayerController* UMLevelManager::GetPlayerController() const
{
	ULocalPlayer* Local = GetLocalPlayer();
	if (Local)
	{
		return Local->GetPlayerController(GetWorld());
	}

	return nullptr;
}
