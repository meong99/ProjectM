#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "MGameplayStatics.generated.h"

class UMViewportClient;
class UPMExperienceManagerComponent;
class APMPlayerControllerBase;
class UMDataTableManager;
class UMDefinitionBase;

UENUM(BlueprintType)
enum class EMCollisionChannel : uint8
{
    Monster		UMETA(DisplayName = "Monster"),
    NPC			UMETA(DisplayName = "NPC"),
    Interaction	UMETA(DisplayName = "Interaction"),
	Player		UMETA(DisplayName = "Player"),
};

UCLASS()
class UMGameplayStatics : public UObject
{
	GENERATED_BODY()

public:
	static UMViewportClient*				GetViewportClient(const UObject* WorldContext);
	static UPMExperienceManagerComponent*	GetExperienceManagerComponent(const UObject* WorldContext);
	static ECollisionChannel				GetCollisionChannel(EMCollisionChannel Channel);
	static UMDataTableManager*				GetDataTableManager();


	// 에디터일땐 에디터팝업, 아닐때는 로그
	static void	ShowErrorOrLog(const FString& Error);

	static bool bShowDebug_Console;
};
