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
	static UMViewportClient* GetViewportClient(const UObject* WorldContext);
	static UPMExperienceManagerComponent* GetExperienceManagerComponent(const UObject* WorldContext);
};