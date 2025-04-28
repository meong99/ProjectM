#pragma once

#include "CoreMinimal.h"
#include "MGameplayStatics.generated.h"

class UMViewportClient;
class UPMExperienceManagerComponent;
class APMPlayerControllerBase;

UCLASS()
class UMGameplayStatics : public UObject
{
	GENERATED_BODY()

public:
	static UMViewportClient*				GetViewportClient(const UObject* WorldContext);
	static UPMExperienceManagerComponent*	GetExperienceManagerComponent(const UObject* WorldContext);

	// 에디터일땐 에디터팝업, 아닐때는 로그
	static void	ShowErrorOrLog(const FString& Error);

	static bool bShowDebug_Console;
};