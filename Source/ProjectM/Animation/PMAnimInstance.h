// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "PMAnimInstance.generated.h"

// BP에 있는거 전부 Native화 해야함
UCLASS()
class PROJECTM_API UPMAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

/*
* Overrided Functions
*/
public:
	UPMAnimInstance();

protected:

/*
* Member Functions
*/
public:

/*
* Member Variables
*/
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = ture), Category = "ProjectM | Character State Data")
	float GroundDistance = -1.0f;
};