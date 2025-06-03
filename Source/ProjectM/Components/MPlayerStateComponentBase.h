// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerStateComponent.h"
#include "GameFramework/PlayerState.h"
#include "MPlayerStateComponentBase.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class PROJECTM_API UMPlayerStateComponentBase : public UPlayerStateComponent
{
	GENERATED_BODY()

/*
* Overrided Function
*/
public:
	UMPlayerStateComponentBase(const FObjectInitializer& ObjectInitializer);

/*
* Member Functions
*/
public:
	template <class T = APlayerController>
	T* GetPlayerController() const
	{
		APlayerState* PlayerState = GetPlayerState<APlayerState>();
		if (PlayerState)
		{
			return Cast<T>(PlayerState->GetPlayerController());
		}

		return nullptr;
	}

	virtual void OnSetNewPawn(APawn* NewPawn, APawn* OldPawn){}
protected:
/*
* Member Variables
*/
protected:
	
};
