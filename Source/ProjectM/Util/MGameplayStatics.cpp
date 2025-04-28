#include "MGameplayStatics.h"
#include "UI/MViewportClient.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "GameFramework/GameStateBase.h"
#include "GameModes/PMExperienceManagerComponent.h"
#include "Misc/MessageDialog.h"

bool UMGameplayStatics::bShowDebug_Console = false;
static FAutoConsoleVariableRef CVarMyBoolVar(
	TEXT("ProjectM.Debug.ShowDebug"), // 콘솔 변수 이름
	UMGameplayStatics::bShowDebug_Console, // 변수와 연동
	TEXT("디버그 옵션을 켤지 말지"), // 설명
	ECVF_Default // 플래그 (옵션)
);

UMViewportClient* UMGameplayStatics::GetViewportClient(const UObject* WorldContext)
{
	UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (World)
	{
		UGameInstance* GameInstance = World->GetGameInstance();
		if (GameInstance)
		{
			return Cast<UMViewportClient>(GameInstance->GetGameViewportClient());
		}
	}

	return nullptr;
}

UPMExperienceManagerComponent* UMGameplayStatics::GetExperienceManagerComponent(const UObject* WorldContext)
{
	UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (World)
	{
		AGameStateBase* GameState = World->GetGameState();
		if (GameState)
		{
			return GameState->FindComponentByClass<UPMExperienceManagerComponent>();
		}
	}

	return nullptr;
}

void UMGameplayStatics::ShowErrorOrLog(const FString& Error)
{
#if WITH_EDITOR
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(Error));
#else
	MCHAE_ERROR(Error);
#endif
}

