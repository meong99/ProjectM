#pragma once

#include "CoreMinimal.h"
#include "PMMappableConfigPair.generated.h"

class UPlayerMappableInputConfig;

USTRUCT()
struct FPMMappableConfigPair
{
	GENERATED_BODY()

	FPMMappableConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};