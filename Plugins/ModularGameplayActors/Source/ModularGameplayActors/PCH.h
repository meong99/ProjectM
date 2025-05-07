#pragma once

#include "CoreMinimal.h"
DECLARE_LOG_CATEGORY_EXTERN(MCHAE_MODULARGAMEPLAYACTORS_CATEGORY, Log, All);
#define MCHAE_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define MCHAE_LOG(Format, ...) UE_LOG(MCHAE_MODULARGAMEPLAYACTORS_CATEGORY, Log, TEXT("%s, %s"), *MCHAE_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define MCHAE_TEST(Format, ...) UE_LOG(MCHAE_MODULARGAMEPLAYACTORS_CATEGORY, Log, TEXT("ForTest : %s, %s"), *MCHAE_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define MCHAE_WARNING(Format, ...) UE_LOG(MCHAE_MODULARGAMEPLAYACTORS_CATEGORY, Warning, TEXT("%s, %s"), *MCHAE_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define MCHAE_ERROR(Format, ...) UE_LOG(MCHAE_MODULARGAMEPLAYACTORS_CATEGORY, Error, TEXT("%s, %s"), *MCHAE_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define MCHAE_FETAL(Format, ...) UE_LOG(MCHAE_MODULARGAMEPLAYACTORS_CATEGORY, Fatal, TEXT("%s, %s"), *MCHAE_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))