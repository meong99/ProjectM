// Fill out your copyright notice in the Description page of Project Settings.

#include "PMAnimInstance.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UPMAnimInstance::UPMAnimInstance()
{
}

void UPMAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
}

void UPMAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	// ASC 내부에서 관리하는 GameplayTag와 AnimInstance의 멤버 Property와 Delegate를 연결하여, 값 변화에 대한 반영을 진행한다.
	GameplayTagPropertyMap.Initialize(this, ASC);
}
