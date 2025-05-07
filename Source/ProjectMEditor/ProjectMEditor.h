#pragma once

#include "Logging/LogMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(MCHAE_EDITOR_CATEGORY, Log, All);

#define _QUOTE(x) #x
#define QUOTE(x) _QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "
#define NOTE( x )  message( __FILE__LINE__" NOTE :   " #x "\n" )
#define TODO( x )  message( __FILE__LINE__" TODO :   " #x "\n" )

#define MCHAE_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define MCHAE_LOG(Format, ...) UE_LOG(MCHAE_EDITOR_CATEGORY, Log, TEXT("%s, %s"), *MCHAE_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define MCHAE_TEST(Format, ...) UE_LOG(MCHAE_EDITOR_CATEGORY, Log, TEXT("ForTest : %s, %s"), *MCHAE_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define MCHAE_WARNING(Format, ...) UE_LOG(MCHAE_EDITOR_CATEGORY, Warning, TEXT("%s, %s"), *MCHAE_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define MCHAE_ERROR(Format, ...) UE_LOG(MCHAE_EDITOR_CATEGORY, Error, TEXT("%s, %s"), *MCHAE_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define MCHAE_FETAL(Format, ...) UE_LOG(MCHAE_EDITOR_CATEGORY, Fatal, TEXT("%s, %s"), *MCHAE_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))