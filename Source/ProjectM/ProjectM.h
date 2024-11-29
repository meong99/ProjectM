// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(MCHAE_CATEGORY, Log, All);
#define MCHAE_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define MCHAE_LOG(Format, ...) UE_LOG(MCHAE_CATEGORY, Log, TEXT("%s, %s"), *MCHAE_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))
#define MCHAE_FETAL(Format, ...) UE_LOG(MCHAE_CATEGORY, Fatal, TEXT("%s, %s"), *MCHAE_CALLINFO, *FString::Printf(TEXT(Format), ##__VA_ARGS__))

//#pragma once
//
//#include "PMGameStateBase.generated.h"
//
//UCLASS()
//class PROJECTM_API PMGameStateBase : public 
//{
//	GENERATED_BODY()
//
///*
//* Engine Functions
//*/
//public:
//	PMGameStateBase();
//
///*
//* Member Functions
//*/
//public:
//
///*
//* Member Variables
//*/
//public:
//};